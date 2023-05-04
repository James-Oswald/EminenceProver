
#include "Formula.hpp"


void Formula::Pred::applyToAsTerm(std::function<void(Term*)> termContext) const{
    Term* dummy = new Term;
    dummy->name = this->name;
    dummy->args = this->args;
    termContext(dummy);
    operator delete(dummy); //Don't call the destructor on dummy, just delete the pointer
}

TermList Formula::Pred::allConstants() const{
    TermList rv;
    applyToAsTerm([&rv](Term* t){rv = t->allConstants();});
    return rv;
}

TermList Formula::Pred::allFunctions() const{
    TermList rv;
    applyToAsTerm([&rv](Term* t){rv = t->allSubfunctions();});
    return rv;
}

size_t Formula::Pred::depth() const{
    size_t rv = 0;
    applyToAsTerm([&rv](Term* t){rv = t->depth();});
    return rv;
}

//With code reuse
/* 

TermList Formula::Pred::allFunctions() const{
    TermList rv;
    for(Term* arg : this->args){
        //.splice(rv.end(),...) functions as a concat
        rv.splice(rv.end(), arg->allFunctions());
    }
    return rv;
}

//Code reuse
size_t Formula::Pred::depth() const{
    size_t termDepth = 0;
    for(Term* arg : this->args){
        size_t argDepth = arg->depth();
        if(argDepth > termDepth){
            termDepth = argDepth;
        }
    }
    return 1 + termDepth;
}

*/