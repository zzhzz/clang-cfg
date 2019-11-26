//
// Created by zzhzz on 2019-09-26.
//

#pragma once

#ifndef CLANG_CFG_PARSEHELPER_H
#define CLANG_CFG_PARSEHELPER_H

#include <string>

#include "extend_cfg/global.h"

namespace clang_cfg {
    using namespace clang;
    using namespace clang::tooling;
    using std::string;
    class ParseHelper {
    public:
        static bool isInSystem(ASTContext& context, const FunctionDecl* decl);
        static bool canIncludeInGraph(Decl* decl);
        static bool canBeCallerInGraph(Decl* decl);
        static string getVarName(const Expr* expr);
        static Decl* getDeclFromCall(CallExpr* callExpr);
    };
}


#endif //CLANG_CFG_PARSEHELPER_H
