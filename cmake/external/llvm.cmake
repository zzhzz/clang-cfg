INCLUDE(ExternalProject)


SET(LLVM_SOURCE_DIR ${THIRD_PARTY_PATH}/llvm)
SET(LLVM_PREFIX ${LLVM_SOURCE_DIR}/project)
INCLUDE_DIRECTORIES(${LLVM_PREFIX}/src/extern_llvm/include)
SET(SOURCE_SUBDIR llvm)

ExternalProject_Add(
        extern_llvm
        GIT_REPOSITORY "https://github.com/llvm/llvm-project.git"
        UPDATE_COMMAND ""
        SOURCE_SUBDIR ${SOURCE_SUBDIR}
        CMAKE_ARGS -DLLVM_ENABLE_PROJECTS=clang
                   -DCMAKE_INSTALL_PREFIX=${LLVM_PREFIX}/build
                   -DLLVM_ENABLE_RTTI=ON
                   -DLLVM_ENABLE_EH=ON
        PREFIX ${LLVM_PREFIX}
        INSTALL_DIR ${LLVM_PREFIX}/build
)

SET(LLVM_PATH ${LLVM_PREFIX}/build)
LINK_DIRECTORIES(${LLVM_PATH}/lib)

INCLUDE_DIRECTORIES(${LLVM_PATH}/include)
SET(LLVM_BIN ${LLVM_PATH}/bin)

LIST(APPEND CLANG_LIBS
    clang
    clangAST
    clangAnalysis
    clangBasic
    clangDriver
    clangEdit
    clangIndex
    clangFrontend
    clangFrontendTool
    clangLex
    clangParse
    clangSema
    clangEdit
    clangASTMatchers
    clangRewrite
    clangRewriteFrontend
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangSerialization
    clangToolingCore
    clangTooling)

LIST(APPEND LLVM_LIBS
        LLVMSupport
        LLVMCore
        LLVMAnalysis
        LLVMOption
        LLVMMCParser
        LLVMBitReader
        LLVMBitstreamReader
        LLVMDemangle
        LLVMProfileData
        LLVMBinaryFormat
        LLVMRemarks
        LLVMMC
        LLVMX86Info
        LLVMX86Utils
        LLVMX86Desc)

