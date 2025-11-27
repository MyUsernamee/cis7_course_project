# CIS-7 Course Project ( BlackJack )

This is a course project that was completed as a requirment of MVC's CIS-7 (Discrete Structures) course. It satisfies the requirements for [Case #4](./prompt.md).

## Installation

To download and use this program, you should be able to obtain a binary from the [releases]() tab on GitHub, or as an [artifact]() in the GitHub actions.

If you can't find a binary there, you can manually compile the program your self as outlined in the [Compiling](#compiling).

## Usage

### TODO: Write usage doc.

## Documentation

All code is documented in the `docs` folder. It contains the required course documentation and other documents outlining the structure of the code. All code should be documented with docstrings.

For comprehensive, up to date, code documentation, use [doxygen](https://www.doxygen.nl/index.html).
Otherwise `doxygen` docs should be generated and stored in the `docs/doxygen/` folder.

## Compiling

There should be auto-compiled binaries in the [releases]() tabs, or as [artifacts]() in GitHub actions.

If you wish to compile locally, you will need:

```
cmake>=3.31.6
(Some c++17 compiler)
```

To actually compile the program will depend on your OS, generally though it follows this process:

```
mkdir build
cd build
cmake ..
(Your favorite build system here)
```

For example on Linux with `make` and `g++`, it should look something like:

```
mkdir build && cd build
cmake .. && make
./blackjack
```
