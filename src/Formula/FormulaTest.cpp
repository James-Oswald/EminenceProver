
#include "Formula.hpp"

#include <cassert>

int main(){
    Formula* f1 = Exists("x", Forall("y", Pred("eq", {Var("x"), Var("y")})));
    assert(f1->formulaDepth() == 3);
    assert(f1->quantifier->var == "x");
    assert(f1->quantifier->arg->subformulae()[0] == f1->subformulae()[0]->quantifier->arg);
    delete f1;
}