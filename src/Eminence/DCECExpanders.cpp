
#include"Expanders.hpp"

/**
 * @brief Converts a top level knowledge operator to a beleif operator
*/
FormulaList KnowledeToBelief(const Formula* formula, const FormulaList& background, const Formula* goal){
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
FormulaList SaysToBelief(const Formula* formula, const FormulaList& background, const Formula* goal){
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
FormulaList PerceptionToKnowledge(const Formula* formula, const FormulaList& background, const Formula* goal){
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
FormulaList BreakupBiConditionals(const Formula* formula, const FormulaList& background, const Formula* goal){
    if(formula->type == Formula::Type::IFF){
        return {formula->binary->left->copy(), formula->binary->right->copy()};
    }else{
        return {};
    }
}

/**
 * @brief replace a top level Not Exists with forall Not
*/
FormulaList NotExistsToForallNot(const Formula* formula, const FormulaList& background, const Formula* goal){
    if(formula->type == Formula::Type::NOT && formula->unary->arg->type == Formula::Type::EXISTS){
        Formula::Quantifier* exists = formula->unary->arg->quantifier;
        return {Forall(exists->var, Not(exists->arg->copy()))};
    }else{
        return {};
    }
}

/**
 * @brief and emimination for modal connectives 
*/
FormulaList ModalConjunctions(const Formula* formula, const FormulaList& background, const Formula* goal){
    //If the top level formula is a conjuction and both subformulae are DCEC modal connectives
    if(formula->type == Formula::Type::AND && 
       formula->binary->left->connectiveType == Formula::ConnectiveType::UNARYTA &&
       formula->binary->right->connectiveType == Formula::ConnectiveType::UNARYTA
    ){
        return {formula->binary->left->copy(), formula->binary->right->copy()};
    }
}

/**
 * @brief if something is common knowledge then all agents know it 
*/
FormulaList ModalConjunctions(const Formula* formula, const FormulaList& background, const Formula* goal){
    
}