
#include "VampireSubprocessProver.hpp"

#include<filesystem>
#include<boost/filesystem.hpp>
#include<boost/process.hpp>
#include<boost/dll/runtime_symbol_info.hpp>
#include<boost/algorithm/string.hpp>

namespace fs = std::filesystem;
namespace ba = boost::algorithm;
namespace bp = boost::process;
namespace bfs = boost::filesystem;
namespace bdll = boost::dll; 

const std::string problemFileName = "EminenceProverProof.p";

bool vamp::solve(const Problem& p){
    //Write the problem to the input file
    static fs::path problemFilePath = fs::temp_directory_path()/problemFileName;
    std::FILE* problemFile = std::fopen(problemFilePath.c_str(),"w");
    if(!problemFile){
        throw std::runtime_error("Failed to open proof temp file:\n" + problemFilePath.string());
    }
    std::string tptpContents = p.toFirstOrderTPTP();
    std::fwrite(tptpContents.c_str(), sizeof(char), tptpContents.length(), problemFile);
    std::fclose(problemFile);

    //run a vampire subprocess
    bp::ipstream vampireOutput;
    //We need to use search_path to force boost to look in the same directory as us
    static bfs::path eminenceProverDir = bdll::program_location().parent_path();
    static bfs::path vampireExecPath = bp::search_path("vampire", {eminenceProverDir}); 
    int resultCode = bp::system(
        vampireExecPath,
        "--input_syntax", "tptp",
        "-om", "szs",
        "--input_file",
        problemFilePath.string(),
        bp::std_out>vampireOutput
    );

    //Process vampire output
    std::string resultString;
    std::getline(vampireOutput, resultString, {});
    //If vampire has exited with a non-zero exit code
    if(resultCode != 0){
        size_t incompleteStrat = resultString.find("incomplete strategy");
        size_t userError = resultString.find("User error");
        if(userError != std::string::npos){
            ba::trim(resultString);
            throw std::runtime_error("Vampire prover exited with a user error! Vampire output:\n" 
                + resultString + "\nProblem Contents:\n" + tptpContents + "\n");
        }else if(incompleteStrat != std::string::npos){
            //Vampire was not able to find a proof due to an incomplete stratagy, return false
            //since we are unable to solve it
            return false;
        }else{
            ba::trim(resultString);
            throw std::runtime_error("Vampire prover exited with an error! Vampire output:\n" 
                + resultString + "\nProblem Contents:\n" + tptpContents + "\n");
        }
    //Vampire exited sucessfully
    }else{
        std::string szsStatusString = "SZS status";
        size_t szsStatusLocationStart = resultString.find(szsStatusString) + szsStatusString.length() + 1;
        if(szsStatusLocationStart == std::string::npos){
            ba::trim(resultString);
            throw std::runtime_error(
                "Vampire completed successfully with an unexpected result! Vampire output:\n" + resultString
            );
        }
        size_t szsStatusLocationEnd = resultString.find(" ", szsStatusLocationStart);
        std::string szsStatus = resultString.substr(szsStatusLocationStart, szsStatusLocationEnd-szsStatusLocationStart);
        if(szsStatus == "Theorem" || szsStatus == "Tautology"){
            return true;
        }else{
            return false;
        }
    }
}