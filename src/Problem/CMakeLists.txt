
add_library(Problem SHARED)

target_link_libraries(Problem Formula FormulaIO)

target_sources(Problem PRIVATE
    Problem.hpp
    Problem.cpp
)