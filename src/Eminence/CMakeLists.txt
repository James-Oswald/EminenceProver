
#TODO we're forced to make this static because our downloaded version of vampire is static
add_library(Eminence SHARED)

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Threads REQUIRED)
#${CMAKE_DL_LIBS} is required for boost::dll 
target_link_libraries(Eminence Threads::Threads Boost::filesystem ${CMAKE_DL_LIBS})

target_sources(Eminence PRIVATE
    EminenceProver.hpp
    EminenceProver.cpp
    Expanders.hpp
    Expanders.cpp
    VampireSubprocessProver.hpp
    VampireSubprocessProver.cpp
)