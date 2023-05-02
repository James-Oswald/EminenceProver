
#include<queue>

#include "Formula.hpp"

Formula::~Formula(){
    switch(this->type){
        case Type::PRED:
            for(Object* arg : this->pred->args){
                delete arg;
            }
            delete this->pred;
            break;
        case Type::NOT:
            delete this->unary->arg;
            delete this->unary;
        case Type::AND:
        case Type::OR:
        case Type::IF:
        case Type::IFF:
            delete this->binary->left;
            delete this->binary->right;
            delete this->binary;
        case Type::FORALL:
        case Type::EXISTS:
            delete this->quantifier->arg;
            delete this->quantifier;
    }
}


std::vector<Formula*> Formula::subformulae() const{
    switch(this->type){
        case Type::PRED:
            return {};
        case Type::NOT:
            return {this->unary->arg};
        case Type::AND:
        case Type::OR:
        case Type::IF:
        case Type::IFF:
            return {this->binary->left, this->binary->right};
        case Type::FORALL:
        case Type::EXISTS:
            return {this->quantifier->arg};
    }
    return {};
}

std::vector<Formula*> Formula::allSubformulae() const{
    std::vector<Formula*> allFormula = {(Formula*)this};
    //Queue based BFS over all nodes
    std::queue<const Formula*> next(std::deque<const Formula*>({this}));
    while(!next.empty()){ 
        for(Formula* subformula: next.front()->subformulae()){
            allFormula.push_back(subformula);
            next.push(subformula);
        }
        next.pop();
    }
    return allFormula;
}

size_t Formula::formulaDepth() const{
    switch(this->type){
        case Type::PRED:
            return 1;
        case Type::NOT:
            return 1 + this->unary->arg->formulaDepth();
        case Type::AND:
        case Type::OR:
        case Type::IF:
        case Type::IFF:
            return 1 + std::max(this->binary->left->formulaDepth(), this->binary->right->formulaDepth());
        case Type::FORALL:
        case Type::EXISTS:
            return 1 + this->quantifier->arg->formulaDepth();
    }
    return -1; 
}

/**
 * Recursively performs an in-order traversal of the formula tree to get a vector of all predicates
 * in the order in which they appear in the formula.
 * @param base the formula to start the in order traversal at
 * @param predicates the list of predicates being built up in the order they appear within base.
*/
void inOrderTraversal(Formula* base, std::vector<Formula*>& predicates){
    switch(base->type){
        case Formula::Type::PRED:
            predicates.push_back(base);
            break;
        case Formula::Type::NOT:
            inOrderTraversal(base->unary->arg, predicates);
            break;
        case Formula::Type::AND:
        case Formula::Type::OR:
        case Formula::Type::IF:
        case Formula::Type::IFF:
            inOrderTraversal(base->binary->left, predicates);
            inOrderTraversal(base->binary->left, predicates);
            break;
        case Formula::Type::FORALL:
        case Formula::Type::EXISTS:
            inOrderTraversal(base->quantifier->arg, predicates);
            break;
    }
}

std::vector<Formula*> Formula::allPredicates() const{
    std::vector<Formula*> predicates;
    inOrderTraversal((Formula*)this, predicates);
    return predicates;
}


bool Formula::isProposition() const{
    return this->type == Type::PRED && this->pred->args.size() == 0;
}


//Construction Helpers =================================================================================================


Formula* Prop(std::string name){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = std::move(name);
    rv->pred->args = std::vector<Object*>();
    return rv;
}

Formula* Pred(std::string name, std::vector<Object*> args){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = std::move(name);
    rv->pred->args = std::move(args);
    return rv;
}

Formula* Not(Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::NOT;
    rv->unary = new Formula::UnaryConnective;
    rv->unary->arg = arg;
    return rv;
}

Formula* And(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::AND;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* Or(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::OR;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* If(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::IF;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* Iff(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::IFF;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* Forall(std::string varName, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::FORALL;
    rv->quantifier = new Formula::Quantifier;
    rv->quantifier->var = std::move(varName);
    rv->quantifier->arg = arg;
    return rv;
}

Formula* Exists(std::string varName, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::EXISTS;
    rv->quantifier = new Formula::Quantifier;
    rv->quantifier->var = std::move(varName);
    rv->quantifier->arg = arg;
    return rv;
}