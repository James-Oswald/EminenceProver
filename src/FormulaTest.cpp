
#include "Formula.hpp"

#include <iostream>

int main(){
    Formula* f1 = Exists("x", Forall("y", Pred("eq", {Var("x"), Var("y")})));
    std::cout<<f1->toString()<<std::endl;
    delete f1;
}