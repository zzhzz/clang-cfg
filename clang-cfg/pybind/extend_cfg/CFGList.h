//
// Created by zzhzz on 2019-09-26.
//

#pragma once

#include "global.h"
#include "data_struct/CFG.h"

namespace clang_cfg{
    class CFGList {
    public:
        static CFGList& getInst() {
            static CFGList inst;
            return inst;
        }
        vector<vector<CFG>> vecs;
    private:
        CFGList() = default;
    };

}
