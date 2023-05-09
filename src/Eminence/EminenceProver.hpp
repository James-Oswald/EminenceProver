

#pragma once

#include<Formula/Formula.hpp>
#include<Problem/Problem.hpp>

/**
 * Top level prover class 
 * @todo Namespace for now... eventually we need to discus how to make a class structure here.
 * so that we can store the vampire and other theorem prover instances as member vars and store setting
 * information.
*/
namespace eminence{

    /**
     * @brief should return iff a proof was found.
    */
    bool solve(Problem p);
}


