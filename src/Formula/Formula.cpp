
#include<queue>
#include<stack>
#include<list>
#include<algorithm>

#include "Formula.hpp"


ObjectList Formula::Pred::allConstants() const{
    ObjectList rv;
    for(Object* arg : this->args){
        //.splice(rv.end(),...) functions as a concat
        rv.splice(rv.end(), arg->allConstants());
    }
    return rv;
}

ObjectList Formula::Pred::allFunctions() const{
    ObjectList rv;
    for(Object* arg : this->args){
        //.splice(rv.end(),...) functions as a concat
        rv.splice(rv.end(), arg->allFunctions());
    }
    return rv;
}

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


FormulaList Formula::subformulae() const{
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

FormulaList Formula::allSubformulae() const{
    FormulaList allFormula;
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
void inOrderPredicateTraversal(Formula* base, FormulaList& predicates){
    switch(base->type){
        case Formula::Type::PRED:
            predicates.push_back(base);
            break;
        case Formula::Type::NOT:
            inOrderPredicateTraversal(base->unary->arg, predicates);
            break;
        case Formula::Type::AND:
        case Formula::Type::OR:
        case Formula::Type::IF:
        case Formula::Type::IFF:
            inOrderPredicateTraversal(base->binary->left, predicates);
            inOrderPredicateTraversal(base->binary->left, predicates);
            break;
        case Formula::Type::FORALL:
        case Formula::Type::EXISTS:
            inOrderPredicateTraversal(base->quantifier->arg, predicates);
            break;
    }
}

FormulaList Formula::allPredicates() const{
    FormulaList predicates;
    inOrderPredicateTraversal((Formula*)this, predicates);
    return predicates;
}

FormulaList Formula::allPropositions() const{
    FormulaList predicates = this->allPredicates();
    //filter for out predicates with args
    std::remove_if(predicates.begin(), predicates.end(), [](Formula* p){return !p->isProposition();});
    return predicates;
}


bool Formula::isProposition() const{
    return this->type == Type::PRED && this->pred->args.size() == 0;
}

/**
 * Recursive in-order traversal function for finding a list of items (Predicates, Constants, Functions)
 * of generic type that are bound by quantifiers.
 * @tparam ItemType the type of item that the quantifer formula will be associated with, Object* in the case
 * of quantifying over objects and functions and Formula* in the case of quantifying over Predicates
 * @param base The formula to start checking from
 * @param quantifierStack An iterable stack of quantifier formulae
 * @param boundObjVars the vector or Object* Formula* pairs the result is written to.
 * @param baseCase a function mapping Predicates to a list of items with names to check if they are bound
 * @param itemName a function mapping an item to its name to check against the quantifier's bound name
*/
template<typename ItemType>
void inOrderQuantifierTraversal(Formula* base, 
                                std::list<Formula*>& quantifierStack,
                                std::list<std::pair<ItemType, Formula*>>& boundObjVars,
                                std::list<ItemType> (*baseCase)(Formula*),
                                std::string (*itemName)(ItemType)){
    switch(base->type){
        //Base Case 1: the formula is a Predicate check if we associate with anything and leave
        case Formula::Type::PRED:
            for(ItemType arg : baseCase(base)){
                for(Formula* quantifierFormula : quantifierStack){
                    if(quantifierFormula->quantifier->var == itemName(arg)){
                        boundObjVars.push_back(std::make_pair(arg, quantifierFormula));
                    }
                }
            }
            break;
        //Recusive case 1: The formula is a quantifier, push onto the quantifier stack to update binding order
        //and traverse the subformula
        case Formula::Type::FORALL:
        case Formula::Type::EXISTS:
            //Push the current quantifer onto the list of bound vars we're looking for
            quantifierStack.push_front(base);
            inOrderQuantifierTraversal(base->quantifier->arg, quantifierStack, boundObjVars, baseCase, itemName);
            //Pop the quantifer since we've traversed all its inner vars
            quantifierStack.pop_front();
            break;
        //Recusive case 2: For any other formula type recurse on all subformulae
        default:
            for(Formula* subformula : base->subformulae()){
                inOrderQuantifierTraversal(subformula, quantifierStack, boundObjVars, baseCase, itemName);
            }
    }
}

std::list<std::pair<Object*, Formula*>> Formula::boundObjectVariables() const{
    std::list<std::pair<Object*, Formula*>> rv;
    std::list<Formula*> quantifierStack;
    auto getConstants = [](Formula* f){return f->pred->allConstants();};
    auto getObjectName = [](Object* o){return o->name;};
    inOrderQuantifierTraversal<Object*>((Formula*)this, quantifierStack, rv, getConstants, getObjectName);
    return rv;
}

std::list<std::pair<Object*, Formula*>> Formula::boundFunctionVariables() const{
    std::list<std::pair<Object*, Formula*>> rv;
    std::list<Formula*> quantifierStack;
    auto getFunctions = [](Formula* f){return f->pred->allFunctions();};
    auto getObjectName = [](Object* o){return o->name;};
    inOrderQuantifierTraversal<Object*>((Formula*)this, quantifierStack, rv, getFunctions, getObjectName);
    return rv;
}

std::list<std::pair<Formula*, Formula*>> Formula::boundPredicateVariables() const{
    std::list<std::pair<Object*, Formula*>> rv;
    std::list<Formula*> quantifierStack;
    auto getPredicates = [](Formula* p){return p;};
    auto getPredicateName = [](Formula* p){return p->pred->name;};
    inOrderQuantifierTraversal<Formula*>((Formula*)this, quantifierStack, rv, getPredicateName);
    return rv;
}

/**
 * Recursive in order traversal function for finding the list of bound object variables and the formulae
 * they bind to. 
 * @param base The formula to start checking from
 * @param quantifiers A list representing an iterable stack of quantifier formulae in the order traversed check 
 * @param boundObjVars the vector or Object* Formula* pairs the result is written to.
*/



//Construction Helpers =================================================================================================


Formula* Prop(std::string name){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = std::move(name);
    rv->pred->args = ObjectList();
    return rv;
}

Formula* Pred(std::string name, ObjectList args){
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