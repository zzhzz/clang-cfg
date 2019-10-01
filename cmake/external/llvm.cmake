INCLUDE(ExternalProject)


SET(LLVM_SOURCE_DIR ${THIRD_PARTY_PATH}/llvm)
SET(LLVM_PREFIX ${LLVM_SOURCE_DIR}/project)
INCLUDE_DIRECTORIES(${LLVM_PREFIX}/build/include)
SET(CMAKE_ARGS "-DLLVM_ENABLE_PROJECTS=clang")
SET(SOURCE_DIR ${LLVM_PREFIX}/src/extern_llvm/llvm)

ExternalProject_Add(
        extern_llvm
        GIT_REPOSITORY "https://github.com/llvm/llvm-project.git"
        UPDATE_COMMAND ""
        SOURCE_DIR "${SOURCE_DIR}"
        CMAKE_ARGS "${CMAKE_ARGS}"
        PREFIX "${LLVM_PREFIX}"
        INSTALL_COMMAND ""
)

ExternalProject_Add_Step(
        extern_llvm
        CONFIG
        COMMAND "cmake -DCMAKE_INSTALL_PREFIX=${LLVM_PREFIX}/build cmake_install.cmake"
        COMMENT "installing llvm"
        DEPENDEES build
        ALWAYS 0
        WORKING_DIRECTORY "${LLVM_PREFIX}/src/extern_llvm-build"
)

