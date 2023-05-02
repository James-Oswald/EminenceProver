
#include "Object.hpp"

Object::~Object(){
    for(Object* arg : args){
        delete arg;
    }
}

bool Object::operator==(const Object& obj){
    if(this->type != obj.type ||
      this->name != obj.name ||
      this->args.size() != obj.args.size())
        return false;
    for(size_t i = 0; i < this->args.size(); i++){
        if(!(*this->args[i] == *obj.args[i])){
            return false;
        }
    }
    return true;
}

Object* Var(std::string name){
    Object* rv = new Object;
    rv->type = Object::Type::VAR;
    rv->name = name;
    rv->args = std::vector<Object*>();
    return rv;
}

Object* Const(std::string name){
    Object* rv = new Object;
    rv->type = Object::Type::CONST;
    rv->name = name;
    rv->args = std::vector<Object*>();
    return rv;
}

Object* Func(std::string name, std::vector<Object*> args){
    Object* rv = new Object;
    rv->type = Object::Type::CONST;
    rv->name = name;
    rv->args = args;
    return rv;
}