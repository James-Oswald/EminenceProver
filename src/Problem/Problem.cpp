
#include"Problem.hpp"

#include<string>
#include<list>
#include<cstdio>
#include<stdexcept>


#include <FormulaIO/writers.hpp>


Problem::Problem(std::list<Formula*> assumptions, Formula* goal)
:assumptions(assumptions), goal(goal){
}

std::string Problem::toFirstOrderTPTP() const{
    std::string rv = "";
    FormulaList::const_iterator itr = assumptions.begin();
    for(size_t i = 0; i < assumptions.size(); i++, itr++){
        std::string name = "assumption" + std::to_string(i);
        rv += FormulaWriter::toFirstOrderTPTP(name, "axiom", *itr) + "\n\n";
    }
    rv += FormulaWriter::toFirstOrderTPTP("goal", "conjecture", goal);
    return rv;
}

void Problem::saveAsTPTP(std::string name) const{
    std::FILE* tptpFile = std::fopen(name.c_str(), "w");
    if(!tptpFile){
        throw std::runtime_error("Could not open file for writing");
    }

    std::string problemContents = this->toFirstOrderTPTP();
    size_t sizeWritten = std::fwrite(problemContents.c_str(), sizeof(char), problemContents.length(), tptpFile);
    if(sizeWritten != problemContents.length()){
        throw std::runtime_error("Did not write to file");
    }
    std::fclose(tptpFile);
}

/**
 * @brief checks if a problem conforms to a formula class restriction, that is, all of its assumptions
 * and goal formulae have some boolean property.
 * @param problem the problem containing formulae to check against
 * @param propertyCallback a callback that takes a formula and returns true if the formula has a property
*/
bool isFormulaClass(const Problem* problem, bool(*propertyCallback)(const Formula*)){
    for(Formula* f : problem->assumptions){
        if(!propertyCallback(f)){
            return false;
        }
    }
    return propertyCallback(problem->goal);
}

bool Problem::isPropositional() const{
    return isFormulaClass(this, [](const Formula* f){return f->isPropositional();});
}

bool Problem::isZerothOrder() const{
    return isFormulaClass(this, [](const Formula* f){return f->isZerothOrder();});
}

bool Problem::isFirstOrder() const{
    return isFormulaClass(this, [](const Formula* f){return f->isFirstOrder();});
}

bool Problem::isSecondOrder() const{
    return isFormulaClass(this, [](const Formula* f){return f->isSecondOrder();});
}