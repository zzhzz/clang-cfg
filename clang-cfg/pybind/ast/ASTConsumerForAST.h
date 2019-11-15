//
// Created by zzhzz on 2019/11/15.
//

#pragma once

#include "global.h"
#include "ASTVisitorForAST.h"
#include "ASTList.h"

#include <vector>

namespace clang_cfg{

    using std::vector;
    class ASTConsumerForAST : public ASTConsumer {
    public:
        ASTConsumerForAST(ASTContext& ctx, Rewriter& rewriter) : visitor(ctx, rewriter) {}

        void HandleTranslationUnit(ASTContext& context) override {
            visitor.TraverseDecl(context.getTranslationUnitDecl());
        }

    private:
        ASTVisitorForAST visitor;
    };
}
