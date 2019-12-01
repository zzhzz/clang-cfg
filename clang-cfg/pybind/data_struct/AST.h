//
// Created by zzhzz on 2019-09-23.
//

#pragma once

#ifndef CLANG_CFG_EXTENDAST_H
#define CLANG_CFG_EXTENDAST_H

#include <vector>
#include <string>
#include <set>
#include <map>
#include <pybind11/stl.h>

using std::vector;
using std::set;
using std::string;
using std::pair;
using std::map;

namespace clang_cfg {
    class AST {
    public:
        void add_node(string label);
        void add_defination(string name);
        void add_usage(string name);
        void add_edge(int u, int v);
        void add_call(int u, string name);
        void modify_node(int node_id, string new_value);
        int get_parent(int uid);
        int get_next();
        string skipspace(string name);
        vector<pair<int, int>> edges;
        vector<string> node_labels;
        set<string> define_vars, use_vars;
        vector<pair<int, string>> calls;
        map<int,int> parent;
        int n;
    };

}


#endif //CLANG_CFG_EXTENDAST_H
