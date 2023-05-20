
#pragma once

#include<Problem/Problem.hpp>

/** @brief Provides utilities for reading Problems and Formula */
namespace readers{
    
    /** @brief reads an entire file into a string */
    std::string readFile(std::string filename);
    
    /**
     * @brief reads a Naveen style "Extensible data notation" (supposedly clojure) DCEC problem file 
     * @param filePath a path to the edn problem file   
     * @return a list of problems stored in the file 
    */
    std::list<Problem> readEdnProblemFile(std::string filePath);
}