//
//
// Created by zzhzz on 2019-09-26.
//
#pragma once

#include <vector>
#include <string>

#include <pybind11/stl.h>

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Analysis/CFG.h>

#include "data_struct/AST.h"

namespace clang_cfg {
    using namespace clang;
    using namespace clang::driver;
    using namespace clang::tooling;
}

