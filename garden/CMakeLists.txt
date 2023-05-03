
#Allows us to use #include<vampire/...> from our sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#[[
#TODO Have yet to get building from source to work

message("[Eminence Prover] Configuring Z3")
add_subdirectory(vampire/Z3)

message("[Eminence Prover] Configuring Vampire")
find_package(Z3 CONFIG REQUIRED) #This is a hack to foce Vampire to find Z3
add_subdirectory(vampire)
]]

SET(LibVampirePath ${CMAKE_CURRENT_BINARY_DIR}/libvampire47.a)
message("Searching for libvampire47.a at " ${CMAKE_CURRENT_BINARY_DIR})
if(NOT EXISTS ${LibVampirePath})
    message("[EmP] Downloading libvampire47.a")
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
        message(FATAL_ERROR "[EmP] Extract libvampire47.a")
    endif()
    message("[EmP] Extracted libvampire47.a")
endif()

message("[EmP] libvampire47.a found")
add_library(Vampire STATIC IMPORTED)
set_target_properties(Vampire PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libvampire47.a)