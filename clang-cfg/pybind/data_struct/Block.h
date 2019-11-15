//
// Created by zzhzz on 2019-09-25.
//
#pragma once

#ifndef CLANG_CFG_BLOCK_H
#define CLANG_CFG_BLOCK_H

#include <vector>
#include <pybind11/stl.h>

#include "AST.h"

using std::vector;

namespace clang_cfg{
    class Block {
    public:
        void add_ast(AST ast);
        vector<AST> content;
    };

}


#endif //CLANG_CFG_BLOCK_H
