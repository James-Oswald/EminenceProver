
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
    ObjectList::const_iterator itr1 = this->args.begin();
    ObjectList::const_iterator itr2 = obj.args.begin();
    for(; itr1 == this->args.end(); itr1++, itr2++){
        if(!(**itr1 == **itr2)){
            return false;
        }
    }
    return true;
}

ObjectList Object::subconstants() const{
    ObjectList rv;
    for(Object* arg : this->args){
        if(arg->args.size() == 0){
            rv.push_back(arg);
        }
    }
    return rv;
}


void inOrderConstantTraversal(Object* base, ObjectList& constants){
    for(Object* arg : base->args){
        if(arg->args.size() == 0){
            constants.push_back(arg);
        }else if(arg->args.size() > 0){
            inOrderConstantTraversal(arg, constants);
        }
    }
}

ObjectList Object::allSubconstants() const {
    ObjectList rv;
    inOrderConstantTraversal((Object *)this, rv);
    return rv;
}

ObjectList Object::allConstants() const{
    if(this->args.size() == 0){
        return {(Object*) this};
    }else{
        return this->allSubconstants();
    }
}

void inOrderFunctionTraversal(Object* base, ObjectList& functions){
    for(Object* arg : base->args){
        if(arg->args.size() != 0){
            functions.push_back(arg);
        }
        inOrderFunctionTraversal(arg, functions);
    }
}

ObjectList Object::allSubfunctions() const {
    ObjectList rv;
    inOrderFunctionTraversal((Object*)this, rv);
    return rv;
}

ObjectList Object::allFunctions() const {
    ObjectList rv = this->allSubfunctions();
    if(rv.size() > 0){
        rv.push_front((Object*)this);
    }
    return rv;
}

// Construction Helpers ================================================================================================

Object* Var(std::string name){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = ObjectList();
    return rv;
}

Object* Const(std::string name){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = ObjectList();
    return rv;
}

Object* Func(std::string name, ObjectList args){
    Object* rv = new Object;
    rv->name = std::move(name);
    rv->args = std::move(args);
    return rv;
}