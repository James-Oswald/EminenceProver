
#pragma once

#include<string>
#include<list>
#include<unordered_set>

struct Term;

using TermList = std::list<Term*>;

/**
 * @brief Represents an term level construct such as constants, term variables, functions with args,
 * and function variables.
 * @details There is no semantic distinction made here at the representation level between 
 * variables and constants. An constant is considered to be a bound variable if it is bound
 * at the formula level, i.e. there exists a quantifier that binds it, otherwise it is a constant.
 * Functions here at the term representational level are just constants with an arity. A function
 * is considered to be a function variable if it is bound at the formula level to a quantifier.
 * 
 * I.E. In this code and documentation we refer to any term with 0 arity (args of length 0) as a "constant"
 * and anything with >0 args as a function, regardless of if they are actually a variable due to being bound
 * at the formula level.
*/
struct Term{

    std::string name; ///< The identifier of the term
    TermList args;  ///< The list of args if function / function variable, else empty

    Term() = default;
    ~Term();

    /** Return true iff two terms are syntactically equivelent */
    bool operator==(const Term& term);

    /**
     * Creates a copy of this term and returns a newly allocated pointer to it.
    */
    Term* copy() const;
    
    /**
     * @brief Return all immediate subconstants of the current term.
     * @example if term is `f(z, g(x), h(x, y), y)` then `.subconstants()` returns a list of pointers to `[z, y]`  
     * @example `x` returns an empty list.
    */
    TermList subconstants() const;

    /**
     * @brief Returns all subconstants 
     * @example if term is `f(z, g(x), h(x, y), y)` then `.subconstants()` returns a list of pointers 
     * [z, x in g(x), x in h(x, y), y in h(x, y), y]
    */
    TermList allSubconstants() const;

    /**
     * @return all subconstants if function or itself if a constant
    */
    TermList allConstants() const;

    /**
     * @brief Returns all subfunctions (excluding the top level function) 
     * @example if term is `f(z, g(x), h(x, i(y)), y)` then `.subfunctions()` returns a list of pointers 
     * to [g(x), h(x, i(y)), i(y)].
    */
    TermList allSubfunctions() const;

    /**
     * @brief Returns all functions (including the top level function) 
     * @example if term is `f(z, g(x), h(x, i(y)), y)` then `.subfunctions()` returns a list of pointers 
     * to `[f(z, g(x), h(x, i(y)), y), g(x), h(x, y), i(y)]`.
    */
    TermList allFunctions() const;

    /**
     * @brief returns the hight of the term tree.
    */
    size_t depth() const;

    /**
     * @brief gets the set of all identifier names in the term. This includes
     * 1) all constant and constant variable names
     * 2) all function and function variable names
    */
    std::unordered_set<std::string> identifiers() const;

};

Term* Var(std::string name);
Term* Const(std::string name);
Term* Func(std::string name, TermList args);
