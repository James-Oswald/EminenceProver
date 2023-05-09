
add_library(Formula SHARED)

target_sources(Formula PRIVATE
    Formula.hpp
    Formula.cpp
    FormulaBuilders.cpp
    Term.hpp
    Term.cpp
    Predicate.cpp
)
