//
// Created by zzhzz on 2019/11/14.
//

#ifndef CLANG_CFG_ASTVISITORFORAST_H
#define CLANG_CFG_ASTVISITORFORAST_H

#include "global.h"
#include "ASTList.h"

#include <vector>

namespace clang_cfg{
    using std::vector;
    using namespace clang;
    using namespace clang::tooling;

    class ASTVisitorForAST : public RecursiveASTVisitor<ASTVisitorForAST> {
    public:
        ASTVisitorForAST(ASTContext& ctx, Rewriter& rewriter) :
                context(ctx), rewriter(rewriter) { };

        bool VisitTranslationUnitDecl(TranslationUnitDecl* decl);

    private:
        ASTContext& context;
        Rewriter& rewriter;
    };
}

#endif //CLANG_CFG_ASTVISITORFORAST_H
