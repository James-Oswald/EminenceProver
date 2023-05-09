
#include "Expanders.hpp"


FormulaList expanders::expand(const Formula* formula, Expander expander){
    return expander(formula);
}


FormulaList expanders::expand(Formula* formula, const ExpanderList& expanders){
    FormulaList rv;
    for(Expander expander : expanders){
        FormulaList expanded = expander(formula);
        rv.splice(rv.end(), expanded);
    }
    return rv;
}

FormulaList expanders::expand(FormulaList formulae, Expander expander){
    FormulaList rv;
    for(Formula* formula : formulae){
        FormulaList expanded = expander(formula);
        rv.splice(rv.end(), expanded);
    }
    return rv;
}

FormulaList expanders::expand(const FormulaList& formulae, const ExpanderList& expanders){
    FormulaList rv;
    for(Formula* formula : formulae){
        for(Expander expander : expanders){
            FormulaList expanded = expander(formula);
            rv.splice(rv.end(), expanded);
        }
    }
    return rv;
}