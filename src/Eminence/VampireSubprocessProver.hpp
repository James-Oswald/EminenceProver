
#pragma once

#include<Problem/Problem.hpp>

/**
 * @brief Functions for interacting with the Vampire Subprocess Prover
*/
namespace vamp {
    /**
     * @param problem a first order problem
     * @return true if the goal of the problem is provable with the given assumptions
    */
    bool solve(const Problem& problem);
}