
#pragma once

#include<string>
#include<vector>

#include"Object.hpp"

/** Represents a formulae with a truth value.
 * 
*/
struct Formula{

    enum class Type{
        PRED,
        NOT,
        AND, OR,
        IF, IFF, 
        FORALL, EXISTS,
    } type;

    struct Pred{
        std::string* name; //Could get away without this being a pointer
        std::vector<Object*>* args; //Could get away without this being a pointer
    };
    struct UnaryConnective{
        Formula* arg;
    };
    struct BinaryConnective{
        Formula* left;
        Formula* right;
    };
    struct Quantifier{
        std::string* var;   //Could get away without this being a pointer
        Formula* arg;
    };

    union{
        Pred* pred;
        UnaryConnective* unary;
        BinaryConnective* binary;
        Quantifier* quantifier;
    };

    Formula() = default;
    ~Formula();
    std::string toString() const;
    std::vector<Formula*> subformula() const;
};

Formula* Prop(std::string name);
Formula* Pred(std::string name, std::vector<Object*> args);
Formula* Not(Formula* arg);
Formula* And(Formula* left, Formula* right);
Formula* Or(Formula* left, Formula* right);
Formula* If(Formula* left, Formula* right);
Formula* Iff(Formula* left, Formula* right);
Formula* Forall(std::string varName, Formula* arg);
Formula* Exists(std::string varName, Formula* arg);
