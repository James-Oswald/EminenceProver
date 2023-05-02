
#pragma once

#include<string>
#include<vector>

#pragma once

/**
 * @brief Represents an object level construct such as constants, object variables, functions with args,
 * and function variables.
 * There is no semantic distinction made here at the representation level between 
 * variables and constants. An constant is considered to be a bound variable if it is bound
 * at the formula level, i.e. there exists a quantifier that binds it, otherwise it is a constant.
 * Functions here at the object representational level are just constants with an arity. A function
 * is considered to be a function variable if it is bound at the formula level to a quantifier.
 * @note In this code and documentation we refer to any object with 0 arity (args of length 0) as a "constant"
 * and anything with >0 args as a function, regardless of if they are actually a variable due to being bound
 * at the formula level.
*/
struct Object{

    std::string name;           ///< The identifier of the constant/variable/function
    std::vector<Object*> args;  ///< 

    Object() = default;
    ~Object();
    
    /**
     * @brief Return all immediate sub-constants of the current object
     * @example if object is `f(z, g(x), h(x, y), y)` then `.subconstants()` returns a list of pointers to `[z, y]`  
     * @example `x` returns an empty list.
    */
    std::vector<Object*> subconstants() const;

    /**
     * @brief Returns all subconstants 
     * @example if object is `f(z, g(x), h(x, y), y)` then `.subconstants()` returns a list of pointers 
     * [z, x in g(x), x in h(x, y), y in h(x, y), y]
    */
    std::vector<Object*> allSubconstants() const;

    /**
     * @brief Returns all subfunctions (excluding the top level function) 
     * @example if object is `f(z, g(x), h(x, y), y)` then `.subfunctions()` returns a list of pointers 
     * to [g(x), h(x, y)].
    */
    std::vector<Object*> allSubfunctions() const;

    bool operator==(const Object& obj);
};

Object* Var(std::string name);
Object* Const(std::string name);
Object* Func(std::string name, std::vector<Object*> args);
