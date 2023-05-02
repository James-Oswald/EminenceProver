
#pragma once

#include<string>
#include<vector>

#pragma once

/**
 * Represents an object level construct.
 * There is no semantic distinction made here at the representation level between 
 * variables and constants. An object is considered to be a bound variable if it is bound
 * at the formula level, i.e. there exists a quantifier that binds it, otherwise it is a constant.
 * 
 * Functions at the representational level are just constants with an arity.
*/
struct Object{

    std::string name;
    std::vector<Object*> args; 

    Object() = default;
    ~Object();
    
    bool operator==(const Object& obj);

};

Object* Const(std::string name);
Object* Func(std::string name, std::vector<Object*> args);
