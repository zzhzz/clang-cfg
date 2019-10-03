//
// Created by zzhzz on 2019-09-26.
//

#include "ASTVisitorForCFG.h"
#include "ParseHelper.h"

namespace clang_cfg {
    using namespace clang;
    using namespace clang::tooling;
    using namespace llvm;

    void ASTVisitorForCFG::addFunction(Decl *d) {
        d = d->getCanonicalDecl();
        FunctionCFG* root = getOrInsertFunction(d);
        root->getCFG(this->context);
    }

    FunctionCFG* ASTVisitorForCFG::getOrInsertFunction(Decl* d) {
        d = d->getCanonicalDecl();
        std::shared_ptr<FunctionCFG>& Node = Roots[d];
        if(!Node){
            Node = std::shared_ptr<FunctionCFG>(new FunctionCFG(d));
            hash[d] = (this->idx)++;
        }
        return Node.get();

    }

    bool ASTVisitorForCFG::VisitFunctionDecl(FunctionDecl *d) {
        if(d->hasBody()){
            if(ParseHelper::isInSystem(this->context, d)){
                return true;
            }
            if(ParseHelper::canBeCallerInGraph(d) && d->isThisDeclarationADefinition()){
                this->addFunction(d);
            }
        }
        return true;
    }
}

