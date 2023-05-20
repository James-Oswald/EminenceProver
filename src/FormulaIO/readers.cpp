
#include <stdexcept>
#include <algorithm>

#include "readers.hpp"
#include "SExpression.hpp"

std::string readers::readFile(std::string filePath){
    FILE* inputFile = std::fopen(filePath.c_str(), "r");
    if(!inputFile){
        throw std::runtime_error("Unable to open requested file:" + filePath);
    }
    std::string returnString;
    std::fseek(inputFile, 0, SEEK_END);
    size_t fileSize = std::ftell(inputFile);
    returnString.resize(fileSize);
    std::rewind(inputFile);
    size_t sizeRead = fread(&returnString[0], 1, fileSize, inputFile);
    if(sizeRead != fileSize){
        throw std::runtime_error("Unable to read requested file:" + filePath);
    }
    return returnString;
}

Term* termFromSExpression();

/**
 * @brief  converts an SExpression to a DCEC formula
 * @param expr the SExpression to convert
*/
Formula* formulaFromSExpression(const SExpression& expr){
    
}

/**
 * @note regarding implementation, this will not work if the problem contains quoted strings that contain
 *       unmatched curly brackets. This a bug that will require more time to deal with and is not breaking 
 *       for any of Naveen's examples.
*/
std::list<Problem> readers::readEdnProblemFile(std::string filePath){
    std::list<Problem> rv;
    std::string fileContents = readers::readFile(filePath);
    
    //Loop through each problem by finding pairs of matching top level brackets
    size_t bracketDepth = 0; //How many nested brackets deep are we?
    size_t startIndex = 0; //Index of the first top level bracket
    for(int i = 0; i < fileContents.length(); i++){
        char c = fileContents[i];
        if(c == '{' && bracketDepth != 0){
            bracketDepth++;
        }else if(c == '{' && bracketDepth == 0){
            bracketDepth++;
            startIndex = i;
        }else if(c == '}' && bracketDepth != 1){
            bracketDepth--;
        }else if(c == '}' && bracketDepth == 1){
            std::string ednProblem = fileContents.substr(startIndex, i - startIndex);
            std::replace(ednProblem.begin(), ednProblem.end(), '{', '(');
            std::replace(ednProblem.begin(), ednProblem.end(), '}', ')');
            sExpression problemExpr(ednProblem);
            sExpression assumptions = problemExpr["assumptions"];
            for(size_t i = 0; i < assumptions.members.size(); i++){
                
            }
            bracketDepth--;
        }
    }
    if(bracketDepth != 0){
        throw std::runtime_error("Bracket mismatch in EDN file:" + filePath);
    }
}