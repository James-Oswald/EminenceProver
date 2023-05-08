
cmake_minimum_required(VERSION 3.16)
project("Parallel Verifier" VERSION 2.0.0)

# Config ======================================================================

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fstack-protector -s")

# Include Paths ===============================================================

include_directories(${CMAKE_SOURCE_DIR}/garden)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Sources =====================================================================

add_subdirectory(garden)    #Submodules 
add_subdirectory(src)       #Our code

# Tests =======================================================================

enable_testing()
add_subdirectory(test)      #Testing
