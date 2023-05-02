
#pragma once

#include<string>
#include<vector>

#include"Object.hpp"

/** 
 * Represents a formulae with a truth value.
*/
struct Formula{

    enum class Type{
        PRED,
        NOT,
        AND, 
        OR,
        IF,
        IFF, 
        FORALL,
        EXISTS,
    } type;

    struct Pred{
        std::string name; //Could get away without this being a pointer
        std::vector<Object*> args; //Could get away without this being a pointer
    };
    struct UnaryConnective{
        Formula* arg;
    };
    struct BinaryConnective{
        Formula* left;
        Formula* right;
    };
    struct Quantifier{
        std::string var;   //Could get away without this being a pointer
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
    
    /** 
     * Returns a vector of pointers to immediate subformulae in left to right order.
     * For example if the formula is `A /\ (B \/ C)` then `.subformulae` returns two pointers
     * to `A` and `(B \/ C)` respectively.
     * @return A vector of Formula* containing immediate subformulae
    */
    std::vector<Formula*> subformulae() const;

    /**
     * Gets all subformulae in the entire tree including the formulae itself. 
     * The vector returned is guaranteed to have the values in breath first traversal order. 
     * For example if the formula is `A /\ (B \/ ~C)` then `.allSubformulae()` will return
     * pointers to `[A /\ (B \/ ~C), A, (B \/ ~C), B, ~C, C]` in that order.
     * @return A vector of Formula* of all subformulae encountered in BFS traversal order of the formula tree. 
    */
    std::vector<Formula*> allSubformulae() const;

    /**
     * Returns the list of all predicates in the order they appear in the formula via an in-order traversal
     * of the formula tree. For example `A /\ (B(a, d) \/ ~C)` returns a list of formula pointers to 
     * `[A, B(a, d), C]` in that order.
    */
    std::vector<Formula*> allPredicates() const;

    /**
     * The maximum depth the formulae is nested.
     * @return the height of the formula tree >= 0
    */
    size_t formulaDepth() const;

    /**
     * Tests if a formula is a 0th order (propositional) logic formula. i.e. 
     * 1) contains only propositional variables, no predicates
     * 2) Exclusively uses the Not, And, Or, If, and Iff connectives
     * @return true iff the formula is a formula from 0th order logic
    */
    bool isZerothOrderFormula() const;


    /**
     * Tests if a 1st order logic formula
     * 1) quantifies only over variables and not predicates. 
     * 2) does not contain any modal operators. 
     * @return true if the formula is minimally first order
    */
    bool isFirstOrderFormula() const;

    /**
     * Tests if a 1st order logic formula
     * @return true if the formula is minimally first order
    */
    bool isSecondOrderFormula() const;

    /**
     * @return true iff the formula is a proposition, that is, a predicate with 0 arguments.
    */
    bool isProposition() const;

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
