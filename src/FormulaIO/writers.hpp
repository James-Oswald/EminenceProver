
#pragma once

#include "../Formula/Formula.hpp"

#include<string>

namespace FormulaWriter{
    std::string toSExpression(Formula* formula);
    std::string toNormalExpression(Formula* formula);
    std::string toFirstOrderTPTP(Formula* formula);
}