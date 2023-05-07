
#TODO we're forced to make this static because our downloaded version of vampire is static
add_library(Eminence SHARED)


target_sources(Eminence PRIVATE
    EminenceProver.hpp
    EminenceProver.cpp
    VampireSubprocessProver.hpp
    VampireSubprocessProver.cpp
)