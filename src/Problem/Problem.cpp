
#include"Problem.hpp"

#include<string>
#include<list>
#include<cstdio>
#include<stdexcept>


#include "../FormulaIO/writers.hpp"


Problem::Problem(std::list<Formula*> assumptions, Formula* goal)
:assumptions(assumptions), goal(goal){
}

std::string Problem::toFirstOrderTPTP(){
    std::string rv = "";
    FormulaList::const_iterator itr = assumptions.begin();
    for(size_t i = 0; i < assumptions.size(); i++, itr++){
        std::string name = "Assumption" + std::to_string(i);
        rv += FormulaWriter::toFirstOrderTPTP(name, "hypothesis", *itr) + "\n\n";
    }
    rv += FormulaWriter::toFirstOrderTPTP("Goal", "goal", goal);
    return rv;
}

void Problem::saveAsTPTP(std::string name){
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