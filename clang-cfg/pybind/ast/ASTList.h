//
// Created by zzhzz on 2019/11/15.
//

#pragma once

#include "global.h"
#include "data_struct/AST.h"

namespace clang_cfg{
    class ASTList {
    public:
        static ASTList& getInst() {
            static ASTList inst;
            return inst;
        }
        vector<AST> vecs;
    private:
        ASTList() = default;
    };

}


