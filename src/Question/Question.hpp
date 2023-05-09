
#pragma once

#include<list>

#include "../Formula/Formula.hpp"

/**
 * @brief represents a question to be answered by a theorem prover
*/
struct Question{
    std::list<Formula*> assumptions;
    Formula* goal;
    std::list<Term*> variablesToAnswer;
    
    Question() = delete;

    Question(std::list<Formula*> assumptions, Formula* goal, std::list<Term*> variablesToAnswer);
    
    std::string toFirstOrderTPTP() const;

    using answer = std::list<std::pair<Term*, Term*>>;
    using answers = std::list<answer>;
};