
set(GrammarSources
    SExpr.cf
)

#Create C++ ASTs for each Grammar Source File
foreach(Source IN ITEMS ${GrammarSources})
    get_filename_component(Source_WE ${Source} NAME_WLE)
    execute_process(
        COMMAND bnfc -o ${Source_WE} --cpp ${Source}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE ExitCode
    )
    if(ExitCode AND NOT ExitCode EQUAL 0)
        message(FATAL_ERROR "Could not run bnfc, make sure it is installed on PATH")
    endif()
endforeach()

