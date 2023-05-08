
# EminenceProver
:spades: Eminence in shadowing :chess_pawn:

## About

EminenceProver is a high-order modal logic theorem prover, particularly designed for reasoning 
in the logics DCEC* and IDCEC. On the backend we make use of a garden of other theorem provers
which currently include:
* Vampire

We intend to someday also include backends for:
* E
* Leo III
* Z3
* SNARK

## Build from source 

With CMake
```bash
git clone --recurse-submodules -j 8 https://github.com/James-Oswald/EminenceProver.git
cd EminenceProver
mkdir build
cd build
cmake ..
make
```

We build the docs separately from the CMake build
```bash
#In the project root 
doxygen Doxyfile
```

## Dependencies

For Building:
* [CMake](https://cmake.org/)
* [Boost C++ Libraries](https://www.boost.org/)
* [Vampire Theorem Prover (included as a submodule)](https://github.com/vprover/vampire)

```bash
#Install on Ubuntu
sudo apt-get install -y libboost-filesystem-dev libboost-dev
```

For Docs:
* [dot (graphviz)](https://graphviz.org/)
* [doxygen](https://www.doxygen.nl/)

```bash
#Install on Ubuntu
sudo apt-get install -y graphviz doxygen
```

Not currently a dependency but was or may be at some point:
* [BNFC](https://github.com/BNFC/bnfc)