
#include<list>

#include "../Formula/Formula.hpp"

struct Problem{
    std::list<Formula*> assumptions;
    Formula* goal;

    /**
     * 
    */
    Problem(std::list<Formula*> assumptions, Formula* goal);
    
    std::string toFirstOrderTPTP() const;
    
    /**
     * @brief saves a problem as a TPTP file.
     * @param filename the path and name of the file to save the problem to.
     * this name is expected to include the file extension in it.
    */
    void saveAsTPTP(std::string fileName) const;

    bool isPropositional() const;
    bool isZerothOrder() const;
    bool isFirstOrder() const;
    bool isSecondOrder() const;
};