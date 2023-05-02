
add_library(Formula SHARED)

target_sources(Formula PRIVATE
    Formula.cpp
    Formula.hpp
    Object.hpp
    Object.cpp
)

add_executable(FormulaTest)
target_link_libraries(FormulaTest Formula)
target_sources(FormulaTest PRIVATE
    FormulaTest.cpp
)