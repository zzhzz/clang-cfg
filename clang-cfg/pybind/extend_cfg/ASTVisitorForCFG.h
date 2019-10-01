//
// Created by zzhzz on 2019-09-26.
//
#pragma once

#ifndef CLANG_CFG_ASTVISITORFORCFG_H
#define CLANG_CFG_ASTVISITORFORCFG_H

#include "global.h"
#include "CFGList.h"
#include "FunctionCFG.h"

#include <vector>

namespace clang_cfg{
    using std::vector;
    using namespace clang;
    using namespace clang::tooling;

    class ASTVisitorForCFG : public RecursiveASTVisitor<ASTVisitorForCFG> {
    public:
        ASTVisitorForCFG(ASTContext& ctx, Rewriter& rewriter) :
                context(ctx), rewriter(rewriter) { idx = 0; };

        void addFunction(Decl* d);
        FunctionCFG* getOrInsertFunction(Decl* d);
        bool VisitFunctionDecl(FunctionDecl* d);

    private:
        ASTContext& context;
        Rewriter& rewriter;
        typedef llvm::DenseMap<const Decl *, std::shared_ptr<FunctionCFG>> RootsMapType;
        RootsMapType Roots;
        int idx;
        std::map<const Decl *, int> hash;
    };
}


#endif //CLANG_CFG_ASTVISITORFORCFG_H
