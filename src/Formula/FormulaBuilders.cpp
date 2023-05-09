
//Construction Helpers 
#include"Formula.hpp"



Formula* Prop(std::string name){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->connectiveType = Formula::ConnectiveType::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = std::move(name);
    rv->pred->args = TermList();
    return rv;
}

Formula* Pred(std::string name, TermList args){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->connectiveType = Formula::ConnectiveType::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = std::move(name);
    rv->pred->args = std::move(args);
    return rv;
}

Formula* Not(Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::NOT;
    rv->connectiveType = Formula::ConnectiveType::UNARY;
    rv->unary = new Formula::UnaryConnective;
    rv->unary->arg = arg;
    return rv;
}

Formula* And(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::AND;
    rv->connectiveType = Formula::ConnectiveType::BINARY;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* Or(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::OR;
    rv->connectiveType = Formula::ConnectiveType::BINARY;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* If(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::IF;
    rv->connectiveType = Formula::ConnectiveType::BINARY;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

Formula* Iff(Formula* left, Formula* right){
    Formula* rv = new Formula;
    rv->type = Formula::Type::IFF;
    rv->connectiveType = Formula::ConnectiveType::BINARY;
    rv->binary = new Formula::BinaryConnective;
    rv->binary->left = left;
    rv->binary->right = right;
    return rv;
}

// Quantification ====================================================================================================

Formula* Forall(std::string varName, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::FORALL;
    rv->connectiveType = Formula::ConnectiveType::QUANT;
    rv->quantifier = new Formula::Quantifier;
    rv->quantifier->var = std::move(varName);
    rv->quantifier->arg = arg;
    return rv;
}

Formula* Exists(std::string varName, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::EXISTS;
    rv->connectiveType = Formula::ConnectiveType::QUANT;
    rv->quantifier = new Formula::Quantifier;
    rv->quantifier->var = std::move(varName);
    rv->quantifier->arg = arg;
    return rv;
}

// DCEC ===============================================================================================================

Formula* Belives(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::BELIEF;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Knows(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::KNOW;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Says(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::SAYS;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Percives(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PERCEP;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Desires(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::DESIRE;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Intends(std::string agent, std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::INTEND;
    rv->connectiveType = Formula::ConnectiveType::UNARYTA;
    rv->unaryTA = new Formula::UnaryTimeAgent;
    rv->unaryTA->agent = std::move(agent);
    rv->unaryTA->time = std::move(time);
    rv->unaryTA->arg = arg;
    return rv;
}

Formula* Common(std::string time, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::SAYS;
    rv->connectiveType = Formula::ConnectiveType::UNARYT;
    rv->unaryT = new Formula::UnaryTime;
    rv->unaryT->time = std::move(time);
    rv->unaryT->arg = arg;
    return rv;
}