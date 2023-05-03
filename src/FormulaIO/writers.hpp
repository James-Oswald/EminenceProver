
#pragma once

#include "../Formula/Formula.hpp"

#include<string>

namespace FormulaWriter{
    std::string toSExpression(Term* formula);
    std::string toSExpression(Formula* formula);
    //std::string toNormalExpression(Formula* formula);
    std::string toFirstOrderTPTP(std::string name, std::string type, Formula* formula);
}