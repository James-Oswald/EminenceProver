
#include<cassert>

#include<Eminence/VampireSubprocessProver.hpp>
#include<Problem/Problem.hpp>
#include<Formula/Formula.hpp>

int main(){
    FormulaList assumptions1 = {Forall("x", Pred("P", {Var("x")}))};
    Formula* goal1 = Pred("P", {Const("a")});
    Problem p1(assumptions1, goal1);
    assert(p1.isFirstOrder());
    assert(vamp::solve(p1) == true);
}