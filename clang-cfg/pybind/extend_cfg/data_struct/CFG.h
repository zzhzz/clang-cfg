//
// Created by zzhzz on 2019-09-25.
//

#ifndef CLANG_CFG_CFG_H
#define CLANG_CFG_CFG_H

#include <vector>
#include <pybind11/stl.h>

#include "Block.h"
#include "AST.h"


namespace clang_cfg {
    using std::vector;
    using std::pair;
    using std::string;

    class CFG{
    public:
        void add_block(Block& block);
        void add_edge(int u, int v, int type);
        vector<pair<pair<int, int>,int>> edges;
        vector<Block> block_list;
        vector<string> call_list;
        string func_name;
    };
}

#endif //CLANG_CFG_CFG_H
