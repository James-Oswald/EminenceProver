
#pragma once

#include<list>

#include "../Formula/Formula.hpp"

/**
 * @brief represents a problem to be solved by a theorem prover
*/
struct Problem{
    std::list<Formula*> assumptions;
    Formula* goal;
    
    Problem() = delete;

    /**
     * @brief construct a new problem from a list of assumptions and a goal.
     * @param assumptions a list of formulae which serve as the assumptions from which we will try to prove the goal.
     * @param goal the formula we will try to prove.
    */
    Problem(std::list<Formula*> assumptions, Formula* goal);
    
    /**
     * @brief Converts a first order problem to contents of a TPTP problem
     * @details will set assumptions to TPTP hypothesis and the goal to a conjecture
     * @return a string which will be a valid TPTP file if saved.  
    */
    std::string toFirstOrderTPTP() const;
    
    /**
     * @brief saves a first order problem as a TPTP file.
     * @param filename the path and name of the file to save the problem to.
     * this name is expected to include the file extension in it.
    */
    void saveAsTPTP(std::string fileName) const;

    /** @brief if the problem is representable in propositional logic */
    bool isPropositional() const;
    /** @brief if the problem is representable in 0th order logic */
    bool isZerothOrder() const;
    /** @brief if the problem is representable in 1st order logic */
    bool isFirstOrder() const;
    /** @brief if the problem is representable in 2nd order logic */
    bool isSecondOrder() const;
};