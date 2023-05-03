add_subdirectory(Formula)
add_subdirectory(FormulaIO)

add_executable(FormulaTest)
target_link_libraries(FormulaTest Formula FormulaIO)
target_sources(FormulaTest PRIVATE
    FormulaTest.cpp
)