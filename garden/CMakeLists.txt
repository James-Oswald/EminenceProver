
#TODO automatically download the submodule if it dosen't exist yet

#Avoid reconfiguring / re-building vampire, manually edit the cache to rebuild
message("[Eminence Prover] Looking for vampire at " ${CMAKE_BINARY_DIR}/bin/vampire)
if(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/vampire)
    message("[Eminence Prover] Vampire not found, will build from source")
    #Compile vampire without Z3 support
    add_subdirectory(vampire)
    set_target_properties(vampire PROPERTIES
        OUTPUT_NAME vampire
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    #Don't build the vampire vtest executable, we don't need it
    set_target_properties(vtest PROPERTIES 
        EXCLUDE_FROM_ALL 1 
        EXCLUDE_FROM_DEFAULT_BUILD 1
    )
else()
    message("[Eminence Prover] Vampire found, will not reconfigure or rebuild")
endif()


#From Back when i thought this was downloading the library rather than the binary exec lmao
#[[
message("[EmP] Searching for Vampire header files at at " ${CMAKE_CURRENT_SOURCE_DIR} "/vampire/Api")
set(VampireAPIPath ${CMAKE_CURRENT_SOURCE_DIR}/vampire/Api/Problem.hpp)
if(NOT EXISTS ${VampireAPIPath})
    #If the vampire files don't exist, make sure the submodule has been initilized
    message("[EmP] Vapire API Not found, attempting to init submodule")
    execute_process(
        COMMAND git submodule update --init 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE ExitCode
    )
    if(${ExitCode} AND NOT ${ExitCode} EQUAL 0)
        message(FATAL_ERROR "[EmP] Failed to update submodule")
    endif()

    #If this didn't work fail
    if(NOT EXISTS ${VampireAPIPath})
        message(FATAL_ERROR "[EmP] Vapire API Not found, Falied")
    endif()
endif()


message("[EmP] found Vampire API")

SET(LibVampirePath ${CMAKE_CURRENT_BINARY_DIR}/libvampire47.a)
message("[EmP] Searching for libvampire47.a at " ${CMAKE_CURRENT_BINARY_DIR})
if(NOT EXISTS ${LibVampirePath})

    #Try to download it if it dosen't exist
    message("[EmP] Not found, downloading libvampire47.a")
    file(DOWNLOAD 
        https://github.com/vprover/vampire/releases/download/v4.7/vampire4.7.zip 
        ${CMAKE_CURRENT_BINARY_DIR}/vampire47.zip
    )
    message("[EmP] Downloaded libvampire47.a")
    message("[EmP] Extracting libvampire47.a")
    execute_process(
        COMMAND bash -c "unzip -o vampire47.zip -d . && mv vampire_z3_rel_static_HEAD_6295 libvampire47.a"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULT_VARIABLE ExitCode
    )
    if(${ExitCode} AND NOT ${ExitCode} EQUAL 0)
        message(FATAL_ERROR "[EmP] Failed to extract libvampire47.a")
    endif()
    message("[EmP] Extracted libvampire47.a")

    #If downloading library still hasn't worked to get the API, fail
    if(NOT EXISTS ${LibVampirePath})
        message(FATAL_ERROR "[EmP] libvampire47.a not found, Falied")
    endif()
endif()

message("[EmP] libvampire47.a found")
#Add it as a library
add_library(Vampire STATIC IMPORTED)
set_target_properties(Vampire PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libvampire47.a)
]]

#[[
SET(VampirePath ${CMAKE_CURRENT_BINARY_DIR}/vampire)
message("[EmP] Searching for Vampire binary at " ${CMAKE_CURRENT_BINARY_DIR})
if(NOT EXISTS ${VampirePath})

    #Try to download it if it dosen't exist
    message("[EmP] Not found, downloading Vampire binary")
    file(DOWNLOAD 
        https://github.com/vprover/vampire/releases/download/v4.7/vampire4.7.zip 
        ${CMAKE_CURRENT_BINARY_DIR}/vampire47.zip
    )
    message("[EmP] Downloaded Vampire binary")
    message("[EmP] Extracting Vampire binary")
    execute_process(
        COMMAND bash -c "unzip -o vampire47.zip -d . && mv vampire_z3_rel_static_HEAD_6295 vampire"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULT_VARIABLE ExitCode
    )
    if(${ExitCode} AND NOT ${ExitCode} EQUAL 0)
        message(FATAL_ERROR "[EmP] Failed to extract Vampire binary")
    endif()
    message("[EmP] Extracted Vampire binary")

    #If downloading library still hasn't worked to get the API, fail
    if(NOT EXISTS ${VampirePath})
        message(FATAL_ERROR "[EmP] libvampire47.a not found, Falied")
    endif()
endif()
#]]