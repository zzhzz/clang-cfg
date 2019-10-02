//
// Created by zzhzz on 2019-09-26.
//
#pragma once

#ifndef CLANG_CFG_CFGFRONTENDACTION_H
#define CLANG_CFG_CFGFRONTENDACTION_H

#include "global.h"
#include "ASTConsumerForCFG.h"

#include <vector>

namespace clang_cfg
{
    using std::vector;
    class CFGFrontendAction : public ASTFrontendAction{
    public:
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef file) override {
            rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
            return std::make_unique<ASTConsumerForCFG>(CI.getASTContext(), rewriter);
        }
    private:
        Rewriter rewriter;

    };
}



#endif //CLANG_CFG_CFGFRONTENDACTION_H
