

add_executable(FormulaTest FormulaTest.cpp)
target_link_libraries(FormulaTest Formula FormulaIO)
add_test(NAME FormulaTest COMMAND FormulaTest)