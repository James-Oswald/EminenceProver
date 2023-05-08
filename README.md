
# Eminence Prover
:spades: Eminence in shadowing :chess_pawn:

## Build from source 
```bash
git clone --recurse-submodules -j 8 https://github.com/James-Oswald/EminenceProver.git
mkdir build
cd build
cmake ..
make
```

We build the docs separately from the CMake build
```
doxygen Doxyfile
```

## Dependencies
For Building:
* [CMake](https://cmake.org/)
* [Boost C++ Libraries](https://www.boost.org/)
* [Vampire Theorem Prover (included as a submodule)](https://github.com/vprover/vampire)
For Docs:
* [doxygen](https://www.doxygen.nl/)
Not currently a dependency but was or may be at some point:
* [BNFC](https://github.com/BNFC/bnfc)