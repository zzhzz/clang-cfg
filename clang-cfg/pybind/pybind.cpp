//
// Created by zzhzz on 2019-09-23.
//

#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <clang/Basic/LangOptions.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include "extend_cfg/data_struct/AST.h"
#include "extend_cfg/data_struct/Block.h"
#include "extend_cfg/data_struct/CFG.h"
#include "extend_cfg/CFGList.h"
#include "extend_cfg/CFGFrontendAction.h"

namespace clang_cfg{

    namespace py = pybind11;

    using std::vector;
    using std::string;

    using namespace clang;
    using namespace clang::tooling;

    std::unique_ptr<CompilationDatabase> getCompilationDataBase(StringRef& config_json) {
        std::string error_str;
        std::unique_ptr<JSONCompilationDatabase> database =
                JSONCompilationDatabase::loadFromBuffer(config_json, error_str, JSONCommandLineSyntax::AutoDetect);
        if(database == NULL) {
            //throw exception
        }
        return database;
    }

    class Parser {
    public:
        vector<CFG> parse(vector<string> file_names, string compile_args, bool with_df) {
            StringRef compile_json = StringRef(compile_args);
            std::unique_ptr<CompilationDatabase> database = getCompilationDataBase(compile_json);
            ClangTool tool(*database, ArrayRef<string>(file_names));

            tool.run(newFrontendActionFactory<CFGFrontendAction>().get());
            CFGList& list = CFGList::getInst();
            vector<CFG> vecs = list.vecs;
            return vecs;
        }
    };

    PYBIND11_MODULE(clang_cfg, m) {
        py::class_<Parser>(m, "Parser")
                .def(py::init<>())
                .def("parse", &Parser::parse)
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


