
#pragma once

#include<string>
#include<list>
#include<functional>

#include"Term.hpp"

class Formula;

using FormulaList = std::list<Formula*>;

/** 
 * @brief Represents a formulae with a truth value.
 * The formula is represented as tree.
*/
struct Formula{

    //Inner Enums / Structs ============================================================================================
    /**
     * @defgroup Inner Enums / Structs
     * Enums and Structs used to help represent the formula tree
    */

    /**
     * @brief The type of the top level operator or connective
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

    /**
     * @brief The class of the top level connective of the current formula
    */
    enum class ConnectiveType{
        PRED,
        UNARY,
        BINARY,
        QUANT,
    };

    struct Pred{
        std::string name;          ///< the identifier for this predicate
        TermList args; ///< the list of arguments to this predicate

        /** @return A list of pointers to all constants (and possible bound variables) inside the predicate */
        TermList allConstants() const;

        /** @return A list of pointers to all functions (and possible function variables) inside the predicate */
        TermList allFunctions() const;

        /** @return max height of terms inside the predicate */
        size_t depth() const;

        /** Provides a memory safe context in which the predicate is interpreted as a term. 
         *  This is useful as it allows us to take advantage of 
         *  @param termContext a lambda which takes the predicate interpreted as a term and does something with it
         *  output can be obtained via binding outside vars by reference.  
        */
        void applyToAsTerm(std::function<void(Term*)> termContext) const;
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
    ConnectiveType connectiveType;  ///< The connective class of the formula
    union{
        Pred* pred;                 ///< Valid iff type == PRED
        UnaryConnective* unary;     ///< Valid iff type == NOT
        BinaryConnective* binary;   ///< Valid iff type == AND, OR, IF, IFF
        Quantifier* quantifier;     ///< Valid iff type == FORALL, EXISTS
    };

    // Methods =========================================================================================
    /** @name Constructors, Destructors, operators */
    ///@{
    
    /** @brief Default constructor, leaves everything uninitialized */
    Formula() = default;

    /** @brief Destructor, frees all subformulae */
    ~Formula();

    /** @brief Returns a pointer to a copy of this formula */
    Formula* copy() const;

    ///@}

    /** @name Subformula & Subterm utilities */
    ///@{

    /** 
     * Returns a vector of pointers to immediate subformulae in left to right order.
     * For example if the formula is `A /\ (B \/ C)` then `.subformulae` returns two pointers
     * to `A` and `(B \/ C)` respectively.
     * @return A vector of Formula* containing immediate subformulae
    */
    FormulaList subformulae() const;

    /**
     * Gets all subformulae in the entire tree excluding the formulae itself. 
     * The vector returned is guaranteed to have the values in breath first traversal order. 
     * For example if the formula is `A /\ (B \/ ~C)` then `.allSubformulae()` will return
     * pointers to `A, (B \/ ~C), B, ~C, C]` in that order.
     * @return A vector of Formula* of all subformulae encountered in BFS traversal order of the formula tree. 
    */
    FormulaList allSubformulae() const;

    /**
     * Returns the list of all predicates in the order they appear in the formula via an in-order traversal
     * of the formula tree. For example `A /\ (B(a, d) \/ ~C)` returns a list of formula pointers to 
     * `[A, B(a, d), C]` in that order.
     * @return a list of Formula* to propositions in the formula in the order they appear 
    */
    FormulaList allPredicates() const;

    /**
     * @return a list of pointer to all term level constants (including constant variables) in the formula
    */
    TermList allConstants() const;

    /**
     * @return a list of pointer to all term level functions (including function variables) in the formula
    */
    TermList allFunctions() const;

    /**
     * @return a list of all subformulae that are quantified at the top level
    */
    FormulaList allQuantified() const;

    /**
     * @return the list of all propositional variable in the order they appear in the formula via an in-order traversal
     * of the formula tree. 
     * @example `A /\ (B(a, d) \/ ~C)` returns a list of formula pointers to 
     * `[A, C]` in that order.
    */
    FormulaList allPropositions() const;

    /**
     * @return a list of pairs term pointers to variables and pointers to the quantifier formula they are bound to
     * @example if formula is `Ex: P(x) /\ Ay: Q(x, y)` then `.boundTermVariables()` returns 
     * `[(pointer to x term in P(x), pointer to quantifier Ex formula), 
     *   (pointer to x term in Q(x,y), pointer to quantifier Ex formula), 
     *   (pointer to y term in Q(x,y), pointer to quantifier Ay formula)]`
     * @example edge case, inner quantifier binds more strongly if shared variable names 
     * if formula is `Ex: P(x) /\ Ax: Q(x, x)` then `.boundTermVariables()` returns 
     * `[(pointer to x term in P(x), pointer to quantifier Ex formula), 
     *   (pointer to 1st x term in Q(x,x), pointer to quantifier Ex formula), 
     *   (pointer to 2nd x term in Q(x,x), pointer to quantifier Ax formula)]`
    */
    std::list<std::pair<Term*, Formula*>> boundTermVariables() const;

    /**
     * @return a list of pairs term pointers to variables and pointers to the quantifier formula they are bound to
    */
    std::list<std::pair<Term*, Formula*>> boundFunctionVariables() const;

    /**
     * @return the list of Predicate variables bound to quantifiers
    */
    std::list<std::pair<Formula*, Formula*>> boundPredicateVariables() const;

    /**
     * gets the set of all identifiers in the formula as strings. Identifiers include:
     * 1) all bound quantifier variable names
     * 2) all term constant and term variable names 
     * 3) all predicate and predicate variable names
     * 4) all all function and function variable names 
    */
    std::unordered_set<std::string> identifiers() const;

    //@}

    /** @name Formula Metrics and Testers */
    ///@{
    
    /**
     * The depth/height of the formula tree.
     * @return the height of the formula tree.
    */
    size_t depth() const;
    
    /**
     * The depth/height of the formula tree, including height of term trees on leaves.
     * @return The depth/height of the formula tree, including height of term trees on leaves.
    */
    size_t depthWithTerms() const;

    /**
     * @return true iff the formula is a proposition, that is, a predicate with 0 arguments.
    */
    bool isProposition() const;

    /**
     * Tests if the formula is a propositional calculus formula.
     * I.E. contains only the connectives: not, and, or, if, iff
     * and contains only propositional variables
     * @return true iff the formula can be interpreted as propositional
    */
    bool isPropositional() const;

    /**
     * Tests if a formula is a 0th order predicate logic formula.
     * I.E. contains only the connectives: not, and, or, if, iff
     * but may contain predicates and terms that are composed of constants
     * @return true iff the formula can be interpreted as zeroth order
    */
    bool isZerothOrder() const;


    /**
     * Tests if a 1st order logic formula i.e
     * I.E. contains only the connectives: not, and, or, if, iff
     * and quantifies over term constants, but not predicates or functions
     * @return true iff the formula can be interpreted as first order
    */
    bool isFirstOrder() const;

    /**
     * Tests if a 2nd order formula i.e
     * Contains quantification over Predicates or Functions
     * @return true iff the formula is minimally 2nd order
    */
    bool isSecondOrder() const;

    //@}
};

// Construction Helpers ================================================================================================

Formula* Prop(std::string name);
Formula* Pred(std::string name, TermList args);
Formula* Not(Formula* arg);
Formula* And(Formula* left, Formula* right);
Formula* Or(Formula* left, Formula* right);
Formula* If(Formula* left, Formula* right);
Formula* Iff(Formula* left, Formula* right);
Formula* Forall(std::string varName, Formula* arg);
Formula* Exists(std::string varName, Formula* arg);

