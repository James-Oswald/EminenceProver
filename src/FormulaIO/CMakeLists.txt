
add_library(FormulaIO SHARED)

target_link_libraries(FormulaIO Formula)

target_sources(FormulaIO PRIVATE
    writers.cpp
    writers.hpp
)