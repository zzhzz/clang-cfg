//
// Created by zzhzz on 2019-09-26.
//

#pragma once

#ifndef CLANG_CFG_CFGLIST_H
#define CLANG_CFG_CFGLIST_H

#include "global.h"
#include "data_struct/CFG.h"

namespace clang_cfg{
    class CFGList {
    public:
        static CFGList& getInst() {
            static CFGList inst;
            return inst;
        }
        vector<CFG> vecs;
    private:
        CFGList() = default;
    };

}


#endif //CLANG_CFG_CFGLIST_H
