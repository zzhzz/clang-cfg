//
// Created by zzhzz on 2019-09-26.
//
#pragma once

#ifndef CLANG_CFG_FUNCTIONCFG_H
#define CLANG_CFG_FUNCTIONCFG_H

#include "global.h"
#include "CFGList.h"

namespace clang_cfg {
    using namespace clang;
    using namespace llvm;
    using std::string;

    class FunctionCFG {
    public:
        typedef FunctionCFG* FunctionRecord;
        FunctionCFG(Decl* d) : decl(d) {}
        string getNameAsString() const;

        void addFunction(FunctionRecord record) {
            if(std::find(functions.begin(), functions.end(), record) != functions.end()) return;
            functions.push_back(record);
        }
        void getCFG(ASTContext& ctx);
        AST transToAST(Stmt* stmt, ASTContext& ctx);
    private:
        Decl* decl;
        SmallVector<FunctionRecord, 10> functions;
    };
}


#endif //CLANG_CFG_FUNCTIONCFG_H
