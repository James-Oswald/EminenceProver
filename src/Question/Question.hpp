
#pragma once

#include<list>

#include "../Formula/Formula.hpp"

/**
 * @brief represents a question to be answered by a theorem prover
*/
struct Question{
    using Answer = std::list<std::pair<Term*, Term*>>;
    using Answers = std::list<Answer>;

    std::list<Formula*> assumptions;
    Formula* goal;
    std::list<Term*> variablesToAnswer;
    Answers restrictions;
    
    Question() = delete;

    Question(std::list<Formula*> assumptions, Formula* goal,
        std::list<Term*> variablesToAnswer, Answers restrictions);
    
    Question(std::list<Formula*> assumptions, Formula* goal,
        std::list<Term*> variablesToAnswer);
    
    std::string toFirstOrderTPTP() const;


};