
#include "Expanders.hpp"


Formula* expanders::expand(const Formula* formula, Expander expander){
    return expander(formula);
}


FormulaList expanders::expand(Formula* formula, const ExpanderList& expanders){
    FormulaList rv;
    for(Expander expander : expanders){
        rv.push_back(expander(formula));
    }
    return rv;
}

FormulaList expanders::expand(FormulaList formulae, Expander expander){
    FormulaList rv;
    for(Formula* formula : formulae){
        rv.push_back(expander(formula));
    }
    return rv;
}

FormulaList expanders::expand(const FormulaList& formulae, const ExpanderList& expanders){
    FormulaList rv;
    for(Formula* formula : formulae){
        for(Expander expander : expanders){
            rv.push_back(expander(formula));
        }
    }
    return rv;
}