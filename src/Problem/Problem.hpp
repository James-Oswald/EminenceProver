
#include<list>

#include "../Formula/Formula.hpp"

struct Problem{
    std::list<Formula*> assumptions;
    Formula* goal;

    Problem(std::list<Formula*> assumptions, Formula* goal);
    std::string toFirstOrderTPTP();
    void saveAsTPTP(std::string fileName);
};