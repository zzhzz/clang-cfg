//
// Created by zzhzz on 2019-09-23.
//

#include "AST.h"

using std::vector;
using std::string;

namespace clang_cfg {
    using std::move;

    int AST::get_next() {
        return (int)node_labels.size();
    }

    string AST::skipspace(string name) {
        for(string::iterator it = name.begin(); it != name.end(); it++){
            if(*it == ' ')
                *it = '_';
        }
        return name;
    }

    void AST::add_node(string label) {
        this->n += 1;
        this->node_labels.emplace_back(skipspace(move(label)));
    }

    void AST::add_defination(string name) {
        if(name.empty()) return;
        this->define_vars.insert(skipspace(move(name)));
    }

    void AST::add_usage(string name) {
        if(name.empty()) return;
        this->use_vars.insert(skipspace(move(name)));
    }

    void AST::add_edge(int u, int v) {
        assert(u < this->n && v < this->n);
        this->edges.emplace_back(std::make_pair(u, v));
    }

    void AST::add_call(int u, string name) {
        this->calls.emplace_back(std::make_pair(u, skipspace(move(name))));
    }

    void AST::modify_node(int node_id, string new_value) {
        this->node_labels[node_id] = new_value;
    }

}


