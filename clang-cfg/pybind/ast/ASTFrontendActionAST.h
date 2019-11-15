//
// Created by zzhzz on 2019/11/14.
//

#pragma once

#include "global.h"
#include "ASTConsumerForAST.h"

#include <vector>

namespace clang_cfg
{
    using std::vector;
    class ASTFrontendActionAST : public ASTFrontendAction {
    public:
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef file) override {
            rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
            return std::make_unique<ASTConsumerForAST>(CI.getASTContext(), rewriter);
        }
    private:
        Rewriter rewriter;

    };
}





