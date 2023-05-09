
#include<cassert>
#include<iostream>

#include<Eminence/VampireSubprocessProver.hpp>
#include<Problem/Problem.hpp>
#include<Question/Question.hpp>
#include<Formula/Formula.hpp>

std::string answerToString(const std::optional<Question::answer> ans) {
    std::string ansStr;
    if (!ans.has_value()) {
        ansStr = "No Answer Found";
    } else {
        const auto ans_value = ans.value();
        for (const auto an : ans_value) {
            ansStr += an.first->name + ", " + an.second->name + "\n";
        }
    }
    return ansStr;
}

int main(){
    FormulaList assumptions1 = {Forall("x", Pred("P", {Var("x")}))};
    Formula* goal1 = Pred("P", {Const("a")});
    Problem p1(assumptions1, goal1);
    assert(!p1.isPropositional());
    assert(!p1.isZerothOrder());
    assert(p1.isFirstOrder());
    assert(p1.isSecondOrder());
    assert(vamp::solve(p1) == true);

    FormulaList assumptions2 = {};
    Formula* goal2 = Iff(Not(And(Prop("A"), Prop("B"))), Or(Not(Prop("A")), Not(Prop("B"))));
    Problem p2(assumptions2, goal2);
    assert(p2.isPropositional());
    assert(p2.isZerothOrder());
    assert(p2.isFirstOrder());
    assert(p2.isSecondOrder());
    assert(vamp::solve(p2) == true);

    FormulaList assumptions3 = {Exists("x", Pred("P", {Var("x")}))};
    Formula* goal3 = Pred("P", {Const("a")});
    Problem p3(assumptions3, goal3);
    assert(!p3.isPropositional());
    assert(!p3.isZerothOrder());
    assert(p3.isFirstOrder());
    assert(p3.isSecondOrder());
    assert(vamp::solve(p3) == false);

    //Rozek Contradictory axioms test
    FormulaList assumptions4 = {
        Forall("x", Pred("lt", {Var("x"), Var("x")})),
        Forall("x", Forall("y", Iff(
            Pred("lt", {Var("x"), Var("y")}),
            Not(Pred("lt", {Var("y"), Var("x")}))
        ))),
    };
    Formula* goal4 = And(Prop("A"), Not(Prop("A")));
    Problem p4(assumptions4, goal4);
    assert(!p4.isPropositional());
    assert(!p4.isZerothOrder());
    assert(p4.isFirstOrder());
    assert(p4.isSecondOrder());
    assert(vamp::solve(p4) == false);

    Formula* P = Prop("P");
    Formula* Q = Prop("Q");

    FormulaList assumptions5 = {Forall("x", Forall("y", Forall("z", Or(Or(Pred("R", {Var("x"), Var("y")}), Pred("R", {Var("z"), Var("y")})), Pred("R", {Var("z"), Var("x")})))))};
    Formula* goal5 = Exists("x", 
        Exists("y", 
            Forall("z", 
                Or(
                    Pred("R", {Var("z"), Var("x")}),
                    Pred("R", {Var("z"), Var("y")})
                )
            )
        )
    );
    Problem p5(assumptions5, goal5);
    assert(vamp::solve(p5) == true);

    // Variables for testing
    Term* X = Var("X");
    Term* Y = Var("Y");

    // Testing single answer extraction for unary predicate
    FormulaList assumptions6 = {Pred("happy", {Const("Brandon")}), Pred("happy", {Const("James")})};
    Formula* goal6 = Pred("happy", {X});
    Question q1(assumptions6, goal6, {X});
    assert(
        answerToString(vamp::extractAnswer(q1))
        ==
        "X, \"Brandon\"\n"
    );

    // Testing single answer extraction for binary predicate
    FormulaList assumptions7 = {Pred("likes", {Const("Brandon"), Const("Clare")}), Pred("happy", {Const("James")})};
    Formula* goal7 = Pred("likes", {X, Y});
    Question q2(assumptions7, goal7, {X, Y});
    assert(
        answerToString(vamp::extractAnswer(q2))
        ==
        "X, \"Brandon\"\nY, \"Clare\"\n"
    );

}