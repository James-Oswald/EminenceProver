
add_library(Question SHARED)

target_link_libraries(Question Formula FormulaIO)

target_sources(Question PRIVATE
    Question.hpp
    Question.cpp
)