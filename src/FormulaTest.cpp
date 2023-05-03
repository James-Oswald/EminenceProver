

#include "Formula/Formula.hpp"
#include "FormulaIO/writers.hpp"

#include<iostream>
#include<memory>
#include<cassert>

//For the sake of testing
using pFormula = std::unique_ptr<Formula>;

int main(){
    pFormula f1 (Exists("x", Forall("y", Pred("eq", {Var("x"), Var("y")}))));
    std::cout<<FormulaWriter::toSExpression(f1.get())<<std::endl;
    std::cout<<FormulaWriter::toFirstOrderTPTP("f1", "hypothesis", f1.get())<<std::endl;
    assert(FormulaWriter::toSExpression(f1.get()) == "(Exists (Forall (eq x y)))");
    assert(f1->depth() == 3);
    assert(f1->depthWithTerms() == 4);
    assert(f1->quantifier->var == "x");
    assert(f1->quantifier->arg->quantifier->var == "y");
    assert(f1->boundTermVariables().size() == 2);
    assert(f1->boundFunctionVariables().size() == 0);
    assert(f1->boundPredicateVariables().size() == 0);

    //Mathmatical Induction in 2nd order logic
    pFormula f2 (
        Forall("P",
            If(
                And(
                    Pred("P", {Const("0")}), //base case
                    Forall("n",              //Inductive step
                        If(
                            Pred("P", {Var("n")}),
                            Pred("P", {Func("add", {Var("n"), Const("1")})})
                        )
                    )
                ),
                Forall("n", Pred("P", {Var("n")}))
            )
        )
    );
    std::cout<<FormulaWriter::toSExpression(f2.get())<<std::endl;
    std::cout<<FormulaWriter::toFirstOrderTPTP("f2", "hypothesis", f2.get())<<std::endl;
    assert(FormulaWriter::toSExpression(f2.get()) == 
          "(Forall (If (And (P 0) (Forall (If (P n) (P (add n 1))))) (Forall (P n))))");
    assert(f2->depth() == 6);
    assert(f2->depthWithTerms() == 8);
    assert(f2->quantifier->var == "P");
    assert(f2->boundTermVariables().size() == 3);
    assert(f2->boundFunctionVariables().size() == 0);
    assert(f2->boundPredicateVariables().size() == 4);
}