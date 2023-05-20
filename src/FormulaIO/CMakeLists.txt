
add_library(FormulaIO SHARED)

target_link_libraries(FormulaIO Formula)

target_sources(FormulaIO PRIVATE
    writers.cpp
    writers.hpp
    readers.hpp
    readers.cpp
    SExpression.hpp
    SExpression.cpp
)