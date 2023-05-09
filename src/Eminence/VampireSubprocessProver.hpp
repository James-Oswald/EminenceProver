
#pragma once

#include<Problem/Problem.hpp>
#include<Question/Question.hpp>
#include <optional>

/**
 * @brief Functions for interacting with the Vampire Subprocess Prover
*/
namespace vamp {
    /**
     * @param problem a first order problem
     * @return true if the problem is satisfiable and a proof has been found
    */
    bool solve(const Problem& problem);

    /**
     * @param problem a first order problem
     * @return An optional list of assignments if exists
    */
    std::optional<Question::answer> extractAnswer(const Question& q);
    std::optional<Question::answers> extractAnswers(const Question& q);
}