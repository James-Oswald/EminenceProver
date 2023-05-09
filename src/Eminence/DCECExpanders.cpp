
#include"Expanders.hpp"

/**
 * @brief Converts a top level knowledge operator to a beleif operator
*/
FormulaList KnowledeToBelief(const Formula* formula){
    Formula* rv = formula->copy();
    if(rv->type == Formula::Type::KNOW){
        rv->type = Formula::Type::BELIEF;
        return {rv};
    }else{
        return {};
    }
}

/**
 * @brief Converts a top level says operator to a beleif operator
*/
FormulaList SaysToBelief(const Formula* formula){
    Formula* rv = formula->copy();
    if(rv->type == Formula::Type::SAYS){
        rv->type = Formula::Type::BELIEF;
        return {rv};
    }else{
        return {};
    }
}

/**
 * @brief Converts a top level says operator to a beleif operator
*/
FormulaList PerceptionToKnowledge(const Formula* formula){
    Formula* rv = formula->copy();
    if(rv->type == Formula::Type::PERCEP){
        rv->type = Formula::Type::KNOW;
        return {rv};
    }else{
        return {};
    }
}

/**
 * @brief replace a top level biconditional with its equivlent conditionals
*/
FormulaList BreakupBiConditionals(const Formula* formula){
    if(formula->type == Formula::Type::IFF){
        return {formula->binary->left->copy(), formula->binary->right->copy()};
    }else{
        return {};
    }
}

/**
 * @brief replace a top level Not Exists with forall Not
*/
FormulaList NotExistsToForallNot(const Formula* formula){
    if(formula->type == Formula::Type::NOT && formula->unary->arg->type == Formula::Type::EXISTS){
        Formula::Quantifier* exists = formula->unary->arg->quantifier;
        return {Forall(exists->var, Not(exists->arg->copy()))};
    }else{
        return {};
    }
}

/**
 * @brief expand 
*/
FormulaList ModalConjunctions(){

}