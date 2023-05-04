
add_library(Formula SHARED)

target_sources(Formula PRIVATE
    Formula.cpp
    Formula.hpp
    Term.hpp
    Term.cpp
    Predicate.cpp
)
