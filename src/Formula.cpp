
#include "Formula.hpp"

Formula::~Formula(){
    switch(this->type){
        case Type::PRED:
            delete this->pred->name;
            for(Object* arg : *(this->pred->args)){
                delete arg;
            }
            delete this->pred->args;
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
            delete this->quantifier->var;
            delete this->quantifier->arg;
            delete this->quantifier;
    }
}

std::string Formula::toString() const{
    switch(this->type){
        case Type::PRED:{
            std::string rv = *(this->pred->name);
            if(this->pred->args->size() > 0){
                rv += '(';
                for(Object* arg : *(this->pred->args)){
                    rv += arg->toString() + ",";
                }
                rv += "\b)";
            }
            return rv;
        }
        case Type::NOT:
            switch(this->unary->arg->type){
                case Type::AND:
                case Type::OR:
                case Type::IF:
                case Type::IFF:
                    return "~(" + this->unary->arg->toString() + ")";
                default:
                    return "~" + this->unary->arg->toString();
            }
        case Type::AND:
            return "(" + this->binary->left->toString() + "/\\" + this->binary->right->toString() + ")";
        case Type::OR:
            return "(" + this->binary->left->toString() + "\\/" + this->binary->right->toString() + ")";
        case Type::IF:
            return "(" + this->binary->left->toString() + "-->" + this->binary->right->toString() + ")";
        case Type::IFF:
            return "(" + this->binary->left->toString() + "<->" + this->binary->right->toString() + ")";
        case Type::FORALL:
            return "(A " + *this->quantifier->var + ":" + this->quantifier->arg->toString() + ")";
        case Type::EXISTS:
            return "(E " + *this->quantifier->var + ":" + this->quantifier->arg->toString() + ")";
    }
    return "ERROR";
}

std::vector<Formula*> Formula::subformula() const{
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

Formula* Prop(std::string name){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = new std::string(std::move(name));
    rv->pred->args = new std::vector<Object*>();
    return rv;
}

Formula* Pred(std::string name, std::vector<Object*> args){
    Formula* rv = new Formula;
    rv->type = Formula::Type::PRED;
    rv->pred = new Formula::Pred;
    rv->pred->name = new std::string(std::move(name));
    rv->pred->args = new std::vector<Object*>(std::move(args));
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
    rv->quantifier->var = new std::string(std::move(varName));
    rv->quantifier->arg = arg;
    return rv;
}

Formula* Exists(std::string varName, Formula* arg){
    Formula* rv = new Formula;
    rv->type = Formula::Type::EXISTS;
    rv->quantifier = new Formula::Quantifier;
    rv->quantifier->var = new std::string(std::move(varName));
    rv->quantifier->arg = arg;
    return rv;
}