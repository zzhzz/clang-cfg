//
// Created by zzhzz on 2019-09-25.
//

#include "Block.h"


namespace clang_cfg {
    void Block::add_ast(clang_cfg::AST ast) {
        this->content.push_back(ast);
    }
}
