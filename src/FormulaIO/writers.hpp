
#pragma once

#include<string>

#include<Formula/Formula.hpp>



/**
 *  @brief Provides utilities for reading and writing Formulae
*/
namespace FormulaWriter{
    std::string toSExpression(Term* formula);
    std::string toSExpression(Formula* formula);
    //std::string toNormalExpression(Formula* formula);
    std::string toFirstOrderTPTP(std::string name, std::string type, Formula* formula);
}