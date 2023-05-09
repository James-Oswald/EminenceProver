
#pragma once

#include<Problem/Problem.hpp>
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

    using answer = std::list<std::pair<Term*, Term*>>;
    using answers = std::list<answer>;

    /**
     * @param problem a first order problem
     * @return An optional list of assignments if exists
    */
    std::optional<answer> extractAnswer(
        const Problem& p, const std::list<Term*>& vars, const answers& forbiddenAnswers);
    std::optional<answers> extractAnswers(const Problem& p, const std::list<Term*>& vars);
}