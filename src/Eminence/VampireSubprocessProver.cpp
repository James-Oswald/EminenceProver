
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
        "-om", "smtcomp",
        "--input_file",
        problemFilePath.string(),
        bp::std_out>vampireOutput
    );

    //Process vampire output and return
    std::string resultString;
    std::getline(vampireOutput, resultString, {});
    ba::trim(resultString);
    if(resultCode != 0){
        throw std::runtime_error("Vampire prover exited with an error! Vampire output:\n" 
            + resultString + "\nProblem Contents:\n" + tptpContents + "\n");
    }
    if(resultString == "sat"){
        return true;
    }else if(resultString == "unsat"){
        return false;
    }else{
        throw std::runtime_error(
            "Vampire completed successfully with an unexpected result! Vampire output:\n" + resultString
        );
    }
}