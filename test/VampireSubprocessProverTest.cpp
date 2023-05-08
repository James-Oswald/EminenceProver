
#include<cassert>

#include<Eminence/VampireSubprocessProver.hpp>
#include<Problem/Problem.hpp>
#include<Formula/Formula.hpp>

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
}