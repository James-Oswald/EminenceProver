
#pragma once

#include<string>
#include<vector>

#include"Object.hpp"

/** 
 * Represents a formulae with a truth value.
 * Note that 
*/
struct Formula{

    //Inner Enums / Structs ============================================================================================
    /**
     * @defgroup Inner Enums / Structs
     * Enums and Structs used to help represent the formula tree
    */

    /**
     * The type of the top level operator or connective
     * in the formation tree.
    */
    enum class Type{
        PRED,       ///< A Predicate or proposition if 0 arguments
        NOT,        ///< Unary connective, logical not
        AND,        ///< Binary connective, logical and
        OR,         ///< Binary connective, logical or
        IF,         ///< Binary connective, conditional
        IFF,        ///< Binary connective, biconditional
        FORALL,     ///< Quantifier, forall
        EXISTS,     ///< Quantifier, exists
    };

    struct Pred{
        std::string name;          ///< the identifier for this predicate
        std::vector<Object*> args; ///< the list of arguments to this predicate

        std::vector<Object*> allSubconstants() const;
        std::vector<Object*> allSubfunctions() const;
    };

    struct UnaryConnective{
        Formula* arg;    ///< formula being bound to this connective
    };

    struct BinaryConnective{
        Formula* left;   ///< left subformula of a binary connective
        Formula* right;  ///< right subformula of a binary connective
    };

    struct Quantifier{
        std::string var;  ///< The identifier (name of the variable) this quantifier binds to
        Formula* arg;     ///< The formula being quantified over
    };

    // Internal Representation =========================================================================================
    /**
     * @defgroup Internal Representation
     * Public members used to represent the formula tree
    */

    Type type;                      ///< The type of the formula
    union{
        Pred* pred;                 ///< Valid iff type == PRED
        UnaryConnective* unary;     ///< Valid iff type == NOT
        BinaryConnective* binary;   ///< Valid iff type == AND, OR, IF, IFF
        Quantifier* quantifier;     ///< Valid iff type == FORALL, EXISTS
    };

    // Methods =========================================================================================
    /** @name Constructors & Destructors */
    ///@{

    Formula() = default;
    ~Formula();

    ///@}

    /** @name Subformula & Subobject utilities */
    ///@{

    /** 
     * Returns a vector of pointers to immediate subformulae in left to right order.
     * For example if the formula is `A /\ (B \/ C)` then `.subformulae` returns two pointers
     * to `A` and `(B \/ C)` respectively.
     * @return A vector of Formula* containing immediate subformulae
    */
    std::vector<Formula*> subformulae() const;

    /**
     * Gets all subformulae in the entire tree excluding the formulae itself. 
     * The vector returned is guaranteed to have the values in breath first traversal order. 
     * For example if the formula is `A /\ (B \/ ~C)` then `.allSubformulae()` will return
     * pointers to `A, (B \/ ~C), B, ~C, C]` in that order.
     * @return A vector of Formula* of all subformulae encountered in BFS traversal order of the formula tree. 
    */
    std::vector<Formula*> allSubformulae() const;

    /**
     * Returns the list of all predicates in the order they appear in the formula via an in-order traversal
     * of the formula tree. For example `A /\ (B(a, d) \/ ~C)` returns a list of formula pointers to 
     * `[A, B(a, d), C]` in that order.
     * @return a list of Formula* to propositions in the formula in the order they appear 
    */
    std::vector<Formula*> allPredicates() const;

    /**
     * @return the list of all propositional variable in the order they appear in the formula via an in-order traversal
     * of the formula tree. 
     * @example `A /\ (B(a, d) \/ ~C)` returns a list of formula pointers to 
     * `[A, C]` in that order.
    */
    std::vector<Formula*> allPropositions() const;

    /**
     * @return a list of pairs object pointers to variables and pointers to the quantifier formula they are bound to
     * @example if formula is `Ex: P(x) /\ Ay: Q(x, y)` then `.boundObjectVariables()` returns 
     * `[(pointer to x object in P(x), pointer to quantifier Ex formula), 
     *   (pointer to x object in Q(x,y), pointer to quantifier Ex formula), 
     *   (pointer to y object in Q(x,y), pointer to quantifier Ay formula)]`
     * @example edge case, inner quantifier binds more strongly if shared variable names 
     * if formula is `Ex: P(x) /\ Ax: Q(x, x)` then `.boundObjectVariables()` returns 
     * `[(pointer to x object in P(x), pointer to quantifier Ex formula), 
     *   (pointer to 1st x object in Q(x,x), pointer to quantifier Ex formula), 
     *   (pointer to 2nd x object in Q(x,x), pointer to quantifier Ax formula)]`
    */
    std::vector<std::pair<Object*, Formula*>> boundObjectVariables() const;

    /**
     * @return a list of pairs object pointers to variables and pointers to the quantifier formula they are bound to
     * @example 
    */
    std::vector<std::pair<Object*, Formula*>> boundFunctionVariables() const;

    /**
     * @return the list of Predicate variables bound to quantifiers
    */
    std::vector<Formula*> boundPredicateVariables() const;

    //@}

    /** @name Formula Metrics and Testers */
    /**
     * The maximum depth the formulae is nested.
     * @return the height of the formula tree >= 0
    */
    size_t formulaDepth() const;

    /**
     * @return true iff the formula is a proposition, that is, a predicate with 0 arguments.
    */
    bool isProposition() const;

    /**
     * Tests if a formula is a 0th order (propositional) logic formula. i.e. 
     * 1) contains only propositional variables, no predicates
     * 2) Exclusively uses the Not, And, Or, If, and Iff connectives
     * @return true iff the formula is a formula from 0th order logic
    */
    bool isZerothOrderFormula() const;


    /**
     * Tests if a 1st order logic formula i.e
     * 1) quantifies only over variables and not predicates or functions. 
     * 2) does not contain any modal operators. 
     * @return true if the formula is minimally 1st order
    */
    bool isFirstOrderFormula() const;

    /**
     * Tests if a 2nd order formula i.e
     * Contains quantification over Predicates or Functions
     * @return true iff the formula is minimally 2nd order
    */
    bool isSecondOrderFormula() const;

    //@}
};

// Construction Helpers ================================================================================================

Formula* Prop(std::string name);
Formula* Pred(std::string name, std::vector<Object*> args);
Formula* Not(Formula* arg);
Formula* And(Formula* left, Formula* right);
Formula* Or(Formula* left, Formula* right);
Formula* If(Formula* left, Formula* right);
Formula* Iff(Formula* left, Formula* right);
Formula* Forall(std::string varName, Formula* arg);
Formula* Exists(std::string varName, Formula* arg);
