# Clang-CFG: A tool based on Clang LibTooling

## Introduction
   The tool is based on clang libtooling technology, used to generate control-flow-graph for C/C++ code.
   
   The tool is a static python library, which could be invoke directly in python.
   
   
## Usage

* Firstly, download the repository
* use `cd clang-cfg` change into the directory
* use `mkdir build` create a directory for binary files
* use `cd build` change into the binary directory
* use `cmake ..` to create MakeFiles
* use `make` to build the project

For an alpha version, I haven't provide python wrapper for the project and shoud be import directly

* run `cd build/clang-cfg/pybind` change into the directory of static library 
* run `python -c "import clang_cfg" to test

## Tips

* This project based on llvm, it will take long time to download and build llvm as a third party project
* This project based on pybind11, which needs to open `-frtti` option of compiler. The default compile option of llvm doesn't open it.