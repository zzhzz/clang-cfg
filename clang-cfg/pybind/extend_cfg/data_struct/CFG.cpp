//
// Created by zzhzz on 2019-09-25.
//

#include "CFG.h"

namespace clang_cfg {
    
    void CFG::add_block(clang_cfg::Block block) {
        this->block_list.push_back(block);
    }
    
    void CFG::add_edge(int u, int v, int type) {
        this->edges.push_back(std::make_pair(std::make_pair(u, v), type));
    }
}
