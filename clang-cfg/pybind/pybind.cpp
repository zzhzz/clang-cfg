//
// Created by zzhzz on 2019-09-23.
//

#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <clang/Basic/LangOptions.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include "data_struct/AST.h"
#include "data_struct/Block.h"
#include "data_struct/CFG.h"
#include "extend_cfg/CFGList.h"
#include "extend_cfg/CFGFrontendAction.h"


#include "ast/ASTFrontendActionAST.h"
#include "ast/ASTList.h"

namespace clang_cfg{

    namespace py = pybind11;

    using std::vector;
    using string = std::string;

    using namespace clang;
    using namespace clang::tooling;

    std::unique_ptr<CompilationDatabase> getCompilationDataBase(StringRef& config_json) {
        string error_str;
        std::unique_ptr<JSONCompilationDatabase> database =
                JSONCompilationDatabase::loadFromBuffer(config_json, error_str, JSONCommandLineSyntax::AutoDetect);
        if(database == NULL) {
            //throw exception
            std::cout << error_str << std::endl;
        }
        return database;
    }

    class Parser {
    public:
        vector<vector<CFG>> parse_to_extendcfg(vector<string> file_names, string compile_args) {
            CFGList& list = CFGList::getInst();
            list.vecs.clear();
            StringRef compile_json = StringRef(compile_args);
            std::unique_ptr<CompilationDatabase> database = getCompilationDataBase(compile_json);
            vector<CompileCommand> vs = (*database).getAllCompileCommands();
            ClangTool tool(*database, ArrayRef<string>(file_names));
            tool.run(newFrontendActionFactory<CFGFrontendAction>().get());
            list = CFGList::getInst();
            vector<vector<CFG>> vecs = list.vecs;
            return vecs;
        }
        vector<AST> parse_to_ast(vector<string> file_names, string compile_args) {
            ASTList& list = ASTList::getInst();
            list.vecs.clear();
            StringRef compile_json = StringRef(compile_args);
            std::unique_ptr<CompilationDatabase> database = getCompilationDataBase(compile_json);
            vector<CompileCommand> vs = (*database).getAllCompileCommands();
            ClangTool tool(*database, ArrayRef<string>(file_names));
            tool.run(newFrontendActionFactory<ASTFrontendActionAST>().get());
            list = ASTList::getInst();
            vector<AST> vecs = list.vecs;
            return vecs;
        }
    };

    PYBIND11_MODULE(clang_cfg, m) {
        py::class_<Parser>(m, "Parser")
                .def(py::init<>())
                .def("parse_to_extendcfg", &Parser::parse_to_extendcfg)
                .def("parse_to_ast", &Parser::parse_to_ast)
                ;

        py::class_<CFG>(m, "CFG")
                .def(py::init<>())
                .def_readwrite("edges", &CFG::edges)
                .def_readwrite("block_list", &CFG::block_list)
                .def_readwrite("call_list", &CFG::call_list)
                .def_readwrite("func_name", &CFG::func_name)
                ;

        py::class_<Block>(m, "Block")
                .def(py::init<>())
                .def_readwrite("content", &Block::content)
                .def_readwrite("sline", &Block::start_lineno)
                .def_readwrite("eline", &Block::end_lineno)
                ;

        py::class_<AST>(m, "AST")
                .def(py::init<>())
                .def_readwrite("edges", &AST::edges)
                .def_readwrite("define_vars", &AST::define_vars)
                .def_readwrite("use_vars", &AST::use_vars)
                .def_readwrite("node_labels", &AST::node_labels)
                .def_readwrite("calls", &AST::calls)
                ;

    }
}


