
#include "writers.hpp"

#include<string>
#include<unordered_map>

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
            Term* dummy = new Term;
            dummy->name = formula->pred->name;
            dummy->args = formula->pred->args;
            std::string rv = toSExpression(dummy);
            operator delete(dummy); //Don't call the destructor on dummy, just delete the pointer
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