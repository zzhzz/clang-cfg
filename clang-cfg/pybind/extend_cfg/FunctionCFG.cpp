//
// Created by zzhzz on 2019-09-26.
//

#include <queue>
#include "FunctionCFG.h"

namespace clang_cfg {
    using namespace llvm;
    using std::string;
    using std::queue;
    using std::pair;
    using std::make_pair;

    string FunctionCFG::getNameAsString() const {
        if(isa<FunctionDecl>(decl)){
            FunctionDecl* functionDecl = dyn_cast<FunctionDecl>(decl);
            return functionDecl->getNameAsString();
        }
    }

    void FunctionCFG::getCFG(clang::ASTContext &ctx) {
        CFG result_cfg;
        result_cfg.func_name = this->getNameAsString();
        std::cout << result_cfg.func_name << std::endl;
        Stmt* body = decl->getBody();
        clang::CFG* cfg = clang::CFG::buildCFG(decl, body, &ctx, clang::CFG::BuildOptions()).get();
        std::cout << cfg->size() << std::endl;
        for(clang::CFG::const_iterator it = cfg->begin(); it != cfg->end(); it++) {
            int block_id = (*it)->getBlockID();
            Block block;
            std::cout << (*it)->size() << std::endl;
            for(clang::CFGBlock::const_iterator block_it = (*it)->begin(); block_it != (*it)->end(); block_it++){
                Optional<clang::CFGStmt> block_stmt = block_it->getAs<clang::CFGStmt>();
                if(block_stmt) {
                    AST ast = transToAST(const_cast<Stmt*>(block_stmt->getStmt()), ctx);
                    block.add_ast(ast);
                }
            }
            int edge_type = 0, edge_size = (*it)->succ_size();
            if(edge_size == 2){
                //True false edge, 1 for false, 2 for true
                edge_type = 1;
            } else if(edge_size == 1){
                //0 for Sequence execute
                edge_type = 0;
            } else {
                // 3 for dataflow, 4 for function call, >5 for switch edges
                edge_type = 5;
            }

            for(clang::CFGBlock::succ_iterator succ_it = (*it)->succ_begin(); succ_it != (*it)->succ_end(); succ_it++){
                CFGBlock* blk = *succ_it;
                if(blk == nullptr)
                    continue;
                int v = blk->getBlockID();
                result_cfg.add_edge(block_id, v, edge_type);
                edge_type += 1;
            }
            result_cfg.add_block(block);
        }
        CFGList& list = CFGList::getInst();
        list.vecs.push_back(result_cfg);
    }

    AST FunctionCFG::transToAST(clang::Stmt* stmt, clang::ASTContext &ctx) {
        AST ast;
        ast.add_node(string(stmt->getStmtClassName()));
        queue<pair<Stmt*, int>> que;
        que.push(make_pair(stmt, 0));
        while(!que.empty()) {
            Stmt* cur_stmt = que.front().first;
            int uid = que.front().second;
            que.pop();
            if(isa<BinaryOperator>(cur_stmt)){
                const BinaryOperator* opstmt = dyn_cast<BinaryOperator>(cur_stmt);
                int v = ast.get_next();
                ast.add_edge(uid, v);
                ast.add_node(string(opstmt->getOpcodeStr().data()));
                std::string lvalue = ParseHelper::getVarName(opstmt->getLHS()->IgnoreParens()->IgnoreImpCasts());
                std::string rvalue = ParseHelper::getVarName(opstmt->getRHS()->IgnoreParens()->IgnoreImpCasts());

                if(opstmt->isAssignmentOp() || opstmt->isCompoundAssignmentOp() || opstmt->isShiftAssignOp()){
                    ast.add_defination(lvalue);
                    ast.add_usage(rvalue);
                } else {
                    ast.add_usage(lvalue);
                    ast.add_usage(rvalue);
                }
            }
            if(isa<UnaryOperator>(cur_stmt)){
                const UnaryOperator* unary = dyn_cast<UnaryOperator>(cur_stmt);
                std::string name = UnaryOperator::getOpcodeStr(unary->getOpcode()).data();
                std::string value = ParseHelper::getVarName(unary->getSubExpr()->IgnoreParens()->IgnoreImpCasts());
                if(unary->isPostfix()){
                    name += "_pre";
                } else {
                    name += "_suf";
                }
                int v = ast.get_next();
                ast.add_edge(uid, v);
                ast.add_node(name);
                ast.add_defination(value);
                ast.add_usage(value);
            }
            if(isa<CallExpr>(cur_stmt)){
                const CallExpr* call_expr = dyn_cast<CallExpr>(cur_stmt);
                std::string name = call_expr->getDirectCallee()->getNameAsString();
                ast.add_call(uid, name);
                int sz = call_expr->getNumArgs();
                for(int i = 0; i < sz; i++){
                    const Expr* arg = call_expr->getArg(i);
                    std::string arg_name = ParseHelper::getVarName(arg->IgnoreImpCasts());
                    ast.add_usage(arg_name);
                }
            }
            if(isa<DeclStmt>(cur_stmt)){
                const DeclStmt* ds = dyn_cast<DeclStmt>(cur_stmt);
                if(ds->isSingleDecl()){
                    const Decl* d = ds->getSingleDecl();
                    if(isa<ValueDecl>(d)){
                        const ValueDecl* vd = dyn_cast<ValueDecl>(d);
                        std::string name = vd->getNameAsString();
                        ast.add_defination(name);
                        std::string type = vd->getType().getUnqualifiedType().getCanonicalType().getAsString();

                        ast.add_edge(uid, ast.get_next());
                        ast.add_node(type);
                    }
                }
            }
            if(isa<DeclRefExpr>(cur_stmt)){
                const DeclRefExpr* dr = dyn_cast<DeclRefExpr>(cur_stmt);
                const ValueDecl* vd = dr->getDecl();
                if(vd){
                    std::string type = vd->getType().getCanonicalType().getAsString();
                    ast.add_edge(uid, ast.get_next());
                    ast.add_node(type);
                }
            }
            if(isa<ImplicitCastExpr>(cur_stmt)){
                const ImplicitCastExpr* expr = dyn_cast<ImplicitCastExpr>(cur_stmt);
                std::string kind = expr->getCastKindName();
                ast.add_edge(uid, ast.get_next());
                ast.add_node(kind);
            }
            if(isa<IntegerLiteral>(cur_stmt)){
                const IntegerLiteral * literal = dyn_cast<IntegerLiteral>(cur_stmt);
                long long v = *(literal->getValue().getRawData());
                ast.modify_node(uid, "const_" + std::to_string(v));
            }
            for(Stmt::child_iterator child = cur_stmt->child_begin(); child != cur_stmt->child_end(); child++){
                if(*child == nullptr){
                    continue;
                }
                string classname = (*child)->getStmtClassName();
                if(classname.length() > 0){
                    que.push(make_pair(*child, ast.get_next()));
                    ast.add_edge(uid, ast.get_next());
                    ast.add_node(classname);
                }
            }
        }
        std::cout << "safe" << std::endl;
        return ast;
    }
}

