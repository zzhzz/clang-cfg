//
// Created by zzhzz on 2019-09-26.
//

#pragma once

#ifndef CLANG_CFG_PARSEHELPER_H
#define CLANG_CFG_PARSEHELPER_H


#include <string>
#include <set>

#include "data_struct/AST.h"
#include "extend_cfg/global.h"

namespace clang_cfg {
    using namespace clang;
    using namespace clang::tooling;
    using std::string;
    using std::set;
    class ParseHelper {
    public:
        static set<string> type_set;
        static void init_type_set();
        static bool isInSystem(ASTContext& context, const FunctionDecl* decl);
        static bool canIncludeInGraph(Decl* decl);
        static bool canBeCallerInGraph(Decl* decl);
        static string getVarName(const Expr* expr);
        static Decl* getDeclFromCall(CallExpr* callExpr);
        static void type_simplify(AST& ast, int uid, string &type);
    };
}


#endif //CLANG_CFG_PARSEHELPER_H
