
#include "VampireSubprocessProver.hpp"
#include "../FormulaIO/writers.cpp"

#include<filesystem>
#include<numeric>
#include<boost/format.hpp>
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

std::optional<vamp::answer> vamp::extractAnswer(
    const Problem& p, const std::list<Term*>& vars, const vamp::answers& forbiddenAnswers) {
    //Write the problem to the input file
    static fs::path problemFilePath = fs::temp_directory_path()/problemFileName;
    std::FILE* problemFile = std::fopen(problemFilePath.c_str(),"w");
    if(!problemFile){
        throw std::runtime_error("Failed to open proof temp file:\n" + problemFilePath.string());
    }

    /*
        Rewriting the problem.

        Since there's no way to output ?[X, Y, Z] in
        the Formula to TPTP class, I'm providing this hacky
        overlay method.

        If you recursively nest (?[X]: ?[Y]: ?[Z]: ...)
        then Vampire will only give an answer to X.

        [1]: We have no way currently of grabbing back the original predicate
        names and the link from makeLegalTPTP.
        [2]: Can't do makeLegalTPTP since that quotes things which we don't want here since
        it's variables.
    */
    std::string varListString = std::accumulate(
         vars.begin(), vars.end(), std::string{},
         [](const std::string& acc, Term* t) -> std::string {
             return acc.empty() ? t->name : acc + ", " + t->name;
         }
     );

    std::string tptpContents = "";

    FormulaList::const_iterator itr = p.assumptions.begin();
    for(size_t i = 0; i < p.assumptions.size(); i++, itr++){
        std::string name = "assumption" + std::to_string(i);
        tptpContents += FormulaWriter::toFirstOrderTPTP(name, "axiom", *itr) + "\n\n";
    }

    tptpContents += boost::str(
        boost::format("fof(goal, question, ?[%1%]: (%2%)).") %
         varListString %
         recursiveToTPTP(p.goal) // TODO: [1] [2]
    );

    // TODO: Need to include way to not allow forbidden answers within problem file


    // BEGIN COPY PASTE
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
        "-av", "off", // EDIT MADE HERE
        "-qa", "answer_literal", // EDIT MADE HERE
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
            return {};
        }else{
            ba::trim(resultString);
            throw std::runtime_error("Vampire prover exited with an error! Vampire output:\n" 
                + resultString + "\nProblem Contents:\n" + tptpContents + "\n");
        }
    }
    
    //Vampire exited sucessfully
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
        
    // END COPY PASTE
        
    if (szsStatus != "Theorem" && szsStatus != "Tautology") {
        return {};
    }
        
    // Extract answer from string

    // Example: % SZS answers Tuple [["two","one"]|_] for question_answer
    std::string szsAnswerString = "SZS answers Tuple [[";
    size_t szsAnswerLocationStart = resultString.find(szsAnswerString) + szsAnswerString.length();
    if(szsStatusLocationStart == std::string::npos){
        ba::trim(resultString);
        throw std::runtime_error(
            "Vampire completed successfully with an unexpected result! Vampire output:\n" + resultString
        );
    }
    size_t szsAnswerLocationEnd = resultString.find("]", szsAnswerLocationStart);
    std::string szsAnswerStr = resultString.substr(szsAnswerLocationStart, szsAnswerLocationEnd-szsAnswerLocationStart);


    // Split the input string into a term list
    std::stringstream ss(szsAnswerStr);
    std::string termStr;
    std::list<Term*> szsAnswer;
    while (std::getline(ss, termStr, ',')) {
        szsAnswer.push_back(Const(termStr));
    }

    if (szsAnswer.size() != vars.size()) {
        // TODO: Throw error
        return {};
    }

    // Combine the answer and variable list to create a mapping
    auto szsAnswerIt = szsAnswer.begin();
    auto varsIt = vars.begin();
    vamp::answer answer;
    while (szsAnswerIt != szsAnswer.end() && varsIt != vars.end()) {
        answer.push_back(std::make_pair(*varsIt, *szsAnswerIt));
        ++szsAnswerIt;
        ++varsIt;
    }
    
    return answer;
}

std::optional<vamp::answers> vamp::extractAnswers(const Problem& p, const std::list<Term*>& vars) {
    vamp::answers priorAnswers;

    bool answerFound = true;
    while (answerFound) {
        std::optional<vamp::answer> ans = extractAnswer(p, vars, priorAnswers);
        if (ans.has_value()) {
            priorAnswers.push_back(ans.value());
        } else {
            answerFound = false;
        }
    }

    if (priorAnswers.size() == 0) {
        return {};
    }

    return priorAnswers;
}
