//
// Created by zzhzz on 2019-09-26.
//

#include <queue>

#include "global.h"
#include "ParseHelper.h"

namespace clang_cfg{
    using namespace clang;
    using namespace llvm;

    bool ParseHelper::isInSystem(ASTContext& context, Decl* decl) {
        if(context.getSourceManager().isInSystemHeader(decl->getLocation()) ||
            context.getSourceManager().isInExternCSystemHeader(decl->getLocation()) ||
            context.getSourceManager().isInSystemMacro(decl->getLocation())) {
            return true;
        }
        return false;
    }

    bool ParseHelper::canIncludeInGraph(Decl* decl) {
        if(isa<FunctionDecl>(decl)){
            const FunctionDecl* functionDecl = dyn_cast<FunctionDecl>(decl);
            if(functionDecl->isDependentContext()) {
                return false;
            }
            IdentifierInfo* identifierInfo = functionDecl->getIdentifier();
            if(identifierInfo && identifierInfo->getName().startswith("__inline")) {
                return false;
            }
        }
        return true;
    }

    bool ParseHelper::canBeCallerInGraph(Decl* decl) {
        if(!decl->hasBody()) {
            return false;
        }
        return canIncludeInGraph(decl);
    }

    string ParseHelper::getVarName(const Expr* expr) {
        std::queue<const Expr*> q;
        q.push(expr);
        while(!q.empty()){
            const Expr* t = q.front();
            q.pop();
            if(isa<DeclRefExpr>(t)){
                const DeclRefExpr* decl = dyn_cast<DeclRefExpr>(t);
                return decl->getNameInfo().getAsString();
            }
            if(isa<ArraySubscriptExpr>(t)){
                const ArraySubscriptExpr* array = dyn_cast<ArraySubscriptExpr>(t);
                q.push(array->getBase()->IgnoreImpCasts());
            }
            if(isa<MemberExpr>(t)){
                const MemberExpr* mem_expr = dyn_cast<MemberExpr>(t);
                q.push(mem_expr->getBase()->IgnoreImpCasts());
            }
        }
        return "";

    }

    Decl* ParseHelper::getDeclFromCall(clang::CallExpr *callExpr) {
        FunctionDecl* functionDecl = callExpr->getDirectCallee();
        if(functionDecl){
            return functionDecl;
        }
        Expr* call_expr = callExpr->getCallee()->IgnoreParenCasts();
        if(isa<BlockExpr>(call_expr)){
            BlockExpr* blockExpr = dyn_cast<BlockExpr>(call_expr);
            return blockExpr->getBlockDecl();
        }
        return nullptr;
    }

}


