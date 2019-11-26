//
// Created by zzhzz on 2019-09-26.
//
#pragma once

#ifndef CLANG_CFG_ASTCONSUMERFORCFG_H
#define CLANG_CFG_ASTCONSUMERFORCFG_H

#include "global.h"
#include "ASTVisitorForCFG.h"
#include "CFGList.h"

#include <vector>

namespace clang_cfg{

    using std::vector;
    class ASTConsumerForCFG : public ASTConsumer {
    public:
        ASTConsumerForCFG(ASTContext& ctx, Rewriter& rewriter) : visitor(ctx, rewriter) {}

        void HandleTranslationUnit(ASTContext& context) override {
            CFGList& list = CFGList::getInst();
            list.vecs.emplace_back(vector<CFG>());
            ParseHelper::init_type_set();
            visitor.TraverseDecl(context.getTranslationUnitDecl());
        }

    private:
        ASTVisitorForCFG visitor;
    };
}

#endif //CLANG_CFG_ASTCONSUMERFORCFG_H
