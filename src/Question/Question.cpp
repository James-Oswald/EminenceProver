
#include"Question.hpp"
#include "../FormulaIO/writers.cpp"

#include<string>
#include<list>
#include<cstdio>
#include<stdexcept>
#include<numeric>

#include<boost/format.hpp>


#include <FormulaIO/writers.hpp>

// Constructors

Question::Question(std::list<Formula*> assumptions, Formula* goal,
 std::list<Term*> variablesToAnswer, Question::Answers restrictions)
    :assumptions(assumptions), goal(goal), variablesToAnswer(variablesToAnswer),
    restrictions(restrictions){
        // TODO: Assert the variables within restrictions match the
        // variables within variablesToAnswer

    };

Question::Question(std::list<Formula*> assumptions, Formula* goal,
    std::list<Term*> variablesToAnswer)
    :assumptions(assumptions), goal(goal), variablesToAnswer(variablesToAnswer){
        // TODO: Assert that variablesToAnswer exist within the goal formula
    };



std::string Question::toFirstOrderTPTP() const {


    /*
        Rewriting the problem.

        Since there's no way to output ?[X, Y, Z] in
        the Formula to TPTP class, I'm providing this hacky
        overlay method.

        If you recursively nest (?[X]: ?[Y]: ?[Z]: ...)
        then Vampire will only give an answer to X.
#include<boost/format.hpp>

        [1]: We have no way currently of grabbing back the original predicate
        names and the link from makeLegalTPTP.
        [2]: Can't do makeLegalTPTP since that quotes things which we don't want here since
        it's variables.
    */

    std::string varListString = std::accumulate(
         variablesToAnswer.begin(), variablesToAnswer.end(), std::string{},
         [](const std::string& acc, Term* t) -> std::string {
             return acc.empty() ? t->name : acc + ", " + t->name;
         }
    );

    std::string tptpContents = "";

    FormulaList::const_iterator itr = assumptions.begin();
    for(size_t i = 0; i < assumptions.size(); i++, itr++){
        std::string name = "assumption" + std::to_string(i);
        tptpContents += FormulaWriter::toFirstOrderTPTP(name, "axiom", *itr) + "\n\n";
    }

    std::string restrictionsStr = "";

    if (restrictions.size() > 0) {

        std::string restrictionsSubStr = "";
        auto restrictionsIt = restrictions.begin();

        for (size_t i = 0; i < restrictions.size(); i++, restrictionsIt++) {
            const Question::Answer& restriction = *restrictionsIt;
            auto restrictionIt = restriction.begin();

            // Establish the restriction (X = "something" & Y = "something2)"
            restrictionsSubStr += "(";
            for (size_t j = 0; j < restriction.size(); j++, restrictionIt++) {
                const auto &[variable, constant] = *restrictionIt;
                restrictionsSubStr += variable->name + " = " + constant->name;
                if (j != restriction.size() - 1) {
                    restrictionsSubStr += " & ";
                }
            }
            restrictionsSubStr += ")";

            // We're OR seperating each of the restrictions
            if (i != restrictions.size() - 1) {
                restrictionsSubStr += " | ";
            }
        }

        restrictionsStr += boost::str(
            boost::format("& ~(%1%)") %
            restrictionsSubStr
        );
    }

    tptpContents += boost::str(
        boost::format("fof(goal, question, ?[%1%]: (%2% %3%)).") %
         varListString %
         recursiveToTPTP(goal) % // TODO: [1] [2]
         restrictionsStr
    );

    return tptpContents;
}
