
#include "writers.hpp"

#include<string>
#include<list>
#include<unordered_map>
#include<unordered_set>
#include<stdexcept>

std::string FormulaWriter::toSExpression(Term* term){
    if(term->args.size() == 0){
        return term->name;
    }else{
        std::string rv = "(" + term->name + " ";
        for(Term* arg : term->args){
            rv += toSExpression(arg) + " ";
        }
        rv.pop_back();
        rv += ")";
        return rv;
    }
}

using TypeStringMap = std::unordered_map<Formula::Type, std::string>;
const TypeStringMap typeStringMap = {
    {Formula::Type::PRED, "Pred"},       
    {Formula::Type::NOT, "Not"},          
    {Formula::Type::AND, "And"},           
    {Formula::Type::OR, "Or"},            
    {Formula::Type::IF, "If"},            
    {Formula::Type::IFF, "Iff"},           
    {Formula::Type::FORALL, "Forall"},        
    {Formula::Type::EXISTS, "Exists"},        
};

std::string FormulaWriter::toSExpression(Formula* formula){
    switch(formula->type)
    {
        case Formula::Type::PRED:{
            std::string rv;
            formula->pred->applyToAsTerm([&rv](Term* t){
                rv = toSExpression(t);
            });
            return rv;
        }    
        default:{
            //If the formula is a valid type, use its type string as its operator, else use "???"
            TypeStringMap::const_iterator itr = typeStringMap.find(formula->type);
            std::string typeString = itr != typeStringMap.end() ? itr->second : "???";
            std::string rv = "(" + typeString + " ";
            for(Formula* arg : formula->subformulae()){
                rv += toSExpression(arg) + " ";
            }
            rv.pop_back();
            rv += ")";
            return rv;
        }
    }
}

// TPTP ================================================================================================================

/**
 * Makes the given formula a "legal" TPTP formula
 * Converts bound variables to 
*/
Formula* makeLegal(Formula* formula){

}

using TPTPStringMapType = std::unordered_map<Formula::Type, std::string>;
const TPTPStringMapType TPTPStringMap = {      
    {Formula::Type::NOT, "~"},          
    {Formula::Type::AND, "&"},           
    {Formula::Type::OR, "|"},            
    {Formula::Type::IF, "=>"},            
    {Formula::Type::IFF, "<=>"},           
    {Formula::Type::FORALL, "!"},        
    {Formula::Type::EXISTS, "?"},        
};

using BoundTermSet = std::unordered_set<Term*>;

std::string recursiveToTPTP(Term* term, BoundTermSet boundTerms){
    if(term->args.size() == 0){
        BoundTermSet::const_iterator itr = boundTerms.find(term);
        if(itr == boundTerms.end()){
            //If we are not bound we are a constant and need to wrap in quotes
            return "\"" + term->name + "\"";
        }else{
            //We are a term variable and not wrapped in quotes 
            return term->name;
        }
    }else{
        std::string rv = term->name + "(";
        for(Term* arg : term->args){
            rv += recursiveToTPTP(arg, boundTerms) + ", ";
        }
        rv.pop_back();
        rv.pop_back();
        rv += ")";
        return rv;
    }
}

std::string recursiveToTPTP(Formula* formula, BoundTermSet boundTerms){
    switch(formula->type){
        case Formula::Type::PRED:{
            std::string rv;
            formula->pred->applyToAsTerm([&rv, boundTerms](Term* t){
                rv = recursiveToTPTP(t, boundTerms);
            });
            return rv;
        }
        case Formula::Type::NOT:
            return TPTPStringMap.at(formula->type) + recursiveToTPTP(formula->unary->arg, boundTerms);
        case Formula::Type::AND:
        case Formula::Type::OR:
        case Formula::Type::IF:
        case Formula::Type::IFF:
            return "(" + recursiveToTPTP(formula->binary->left, boundTerms) + TPTPStringMap.at(formula->type) +
                   recursiveToTPTP(formula->binary->right, boundTerms) + ")";
        case Formula::Type::FORALL:
        case Formula::Type::EXISTS:
            return "(" + TPTPStringMap.at(formula->type) + " [" + formula->quantifier->var + "] : " +
                   recursiveToTPTP(formula->quantifier->arg, boundTerms) + ")"; 
    }
    return "???";
}

std::string FormulaWriter::toFirstOrderTPTP(std::string name, std::string type, Formula* formula){

    if(!formula->isFirstOrderFormula()){
        throw std::runtime_error("Trying to convert a non-first order formula to first order TPTP");
    }

    BoundTermSet boundTerms;
    for(auto [term, quantifierFormula] : formula->boundTermVariables()){
        boundTerms.insert(term);
    }
    return "fof(" + name + "," + type + "," + recursiveToTPTP(formula, boundTerms) + ").";
}