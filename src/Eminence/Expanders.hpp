
#include<list>

#include<Formula/Formula.hpp>

/** @brief an expander is a function that takes a formula and expands it acording to some rule,
 *  returning an expanded copy */
using Expander = Formula* (*)(const Formula*);

/** @brief an expander list is a std::list of expanders */
using ExpanderList = std::list<Expander>;


/**
 * @brief namespace for formula expanders and expander helpers
 * @details expanders function as 
*/
namespace expanders{

    // Expander Lists ==================================================================================================
    /**
     * @brief a global list of expanders used by Naveen for DCEC
    */
    const ExpanderList NaveenDCECExpanders;

    // Functions for expansion application =============================================================================
    /** @brief apply a single expander to a formula 
     *  @return a newly allocated formula
    */
    Formula* expand(const Formula* formula, Expander expander);

    /** @brief apply a list of expanders to a formula to get a new formula list */
    FormulaList expand(Formula* formula, const ExpanderList& expanders);

    /** @brief apply a single expander to a list of formulae*/
    FormulaList expand(FormulaList formulae, Expander expander);

    /** @brief apply expander list to a list of formulae
     *  tries to apply each expander to each formulae
    */
    FormulaList expand(const FormulaList& formulae, const ExpanderList& expanders);

}