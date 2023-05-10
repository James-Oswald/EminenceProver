
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

    std::string tptpContents = "";

    // Write out all assumptions
    FormulaList::const_iterator itr = assumptions.begin();
    for(size_t i = 0; i < assumptions.size(); i++, itr++){
        std::string name = "assumption" + std::to_string(i);
        tptpContents += FormulaWriter::toFirstOrderTPTP(name, "axiom", *itr) + "\n\n";
    }


    boost::format tptpFormat("fof(goal, question, ?[%1%]: (%2% %3%)).");

    // %1%: String list of variable names
    const std::string varListString = std::accumulate(
         variablesToAnswer.begin(), variablesToAnswer.end(), std::string{},
         [](const std::string& acc, Term* t) -> std::string {
             return acc.empty() ? t->name : acc + ", " + t->name;
         }
    );

    // %2%: TPTP Goal
    const std::string tptpGoal = recursiveToTPTP(goal); // NOTE: [1] [2]


    // %3%: Restrictions on answers
    // Example string: ~((X = "a" & Y = "b" ) | (X = "b" & Y = "a"))
    std::string restrictionsStr = "";

    if (restrictions.size() > 0) {

        boost::format restrictionsFormat("& ~(%1%)");
        std::string restrictionsSubStr = "";
        Question::Answers::const_iterator restrictionsIt = restrictions.begin();

        size_t i = 0;
        for (const Question::Answer restriction: restrictions) {
            Question::Answer::const_iterator restrictionIt = restriction.begin();

            // Establish the restriction (X = "something" & Y = "something2)"
            restrictionsSubStr += "(";
            size_t j = 0;
            for (const auto &[variable, constant] : restriction) {
                restrictionsSubStr += variable->name + " = " + constant->name;

                // AND separating each assignment within a restriction
                if (j != restriction.size() - 1) {
                    restrictionsSubStr += " & ";
                }
                j++;
            }
            restrictionsSubStr += ")";

            // We're OR seperating each of the restrictions
            if (i != restrictions.size() - 1) {
                restrictionsSubStr += " | ";
            }
            i++;
        }

        restrictionsStr += boost::str(
            restrictionsFormat %
            restrictionsSubStr
        );
    }

    tptpContents += boost::str(
        tptpFormat %
        varListString %
        tptpGoal % 
        restrictionsStr
    );

    return tptpContents;
}
