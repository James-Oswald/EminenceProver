
#include "Object.hpp"

Object::~Object(){
    for(Object* arg : args){
        delete arg;
    }
}

/**
 * Returns true iff the objects share the same name and, in the event they have arguments,
 * if they share the same arguments.
 * @return if the objects are semantically equivelent 
*/
bool Object::operator==(const Object& obj){
    //Name and argument size are the same.
    if(this->name != obj.name || this->args.size() != obj.args.size())
        return false;
    //Arguments are the same
    for(size_t i = 0; i < this->args.size(); i++){
        if(!(*this->args[i] == *obj.args[i])){
            return false;
        }
    }
    return true;
}

std::vector<Object*> Object::subconstants() const{
    std::vector<Object*> rv;
    for(Object* arg : this->args){
        if(arg->args.size() == 0){
            rv.push_back(arg);
        }
    }
    return rv;
}

void inOrderConstantTraversal(std::vector<Object*> constants){

}

std::vector<Object*> Object::allSubconstants() const {
    std::vector<Object*> rv;
    
}

// Construction Helpers ================================================================================================

Object* Var(std::string name){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = std::vector<Object*>();
    return rv;
}

Object* Const(std::string name){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = std::vector<Object*>();
    return rv;
}

Object* Func(std::string name, std::vector<Object*> args){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = std::move(args);
    return rv;
}