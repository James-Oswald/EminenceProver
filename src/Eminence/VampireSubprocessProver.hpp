
#pragma once

#include<Problem/Problem.hpp>

/**
 * @brief Functions for interacting with the Vampire Subprocess Prover
*/
namespace vamp {
    /**
     * @param problem a first order problem
     * @return true if the problem is satisfiable and a proof has been found
    */
    bool solve(const Problem& problem);
}