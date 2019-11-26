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
        if(const FunctionDecl* functionDecl = dyn_cast<FunctionDecl>(decl)){
            return functionDecl->getNameAsString();
        }
    }

    void FunctionCFG::getCFG(clang::ASTContext &ctx) {
        CFG result_cfg;
        result_cfg.func_name = this->getNameAsString();
        Stmt* body = decl->getBody();
        std::unique_ptr<clang::CFG> cfg_ptr = clang::CFG::buildCFG(decl, body, &ctx, clang::CFG::BuildOptions());
        clang::CFG* cfg = cfg_ptr.get();
        for(clang::CFG::const_iterator it = cfg->begin(); it != cfg->end(); it++) {
            clang::CFGBlock* blk = *it;
            if(blk == nullptr)
                continue;
            int block_id = blk->getBlockID();
            Block block;
            for(clang::CFGBlock::const_iterator block_it = blk->begin(); block_it != blk->end(); block_it++){
                Optional<clang::CFGStmt> block_stmt = block_it->getAs<clang::CFGStmt>();
                if(block_stmt) {
                    Stmt* stmt = const_cast<Stmt*>(block_stmt->getStmt());
                    if(stmt != nullptr){
                        AST ast = transToAST(stmt, ctx);
                        block.add_ast(ast);
                    }
                }
            }
            int edge_type = 0, edge_size = blk->succ_size();
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
            for(clang::CFGBlock::succ_iterator succ_it = blk->succ_begin(); succ_it != blk->succ_end(); succ_it++){
                CFGBlock* succ_blk = *succ_it;
                if(succ_blk == nullptr)
                    continue;
                int v = succ_blk->getBlockID();
                result_cfg.add_edge(block_id, v, edge_type);
                edge_type += 1;
            }
            result_cfg.add_block(block);
        }
        CFGList& list = CFGList::getInst();
        list.vecs.back().push_back(result_cfg);
    }

    AST FunctionCFG::transToAST(clang::Stmt* stmt, clang::ASTContext &ctx) {
        AST ast;
        string name = stmt->getStmtClassName();
        ast.add_node(name);
        queue<pair<Stmt*, int>> que;
        while(!que.empty()) que.pop();
        que.push(make_pair(const_cast<Stmt*>(stmt), 0));
        while(!que.empty()) {
            Stmt* cur_stmt = que.front().first;
            int uid = que.front().second;
            que.pop();
            if(const BinaryOperator* opstmt = dyn_cast<BinaryOperator>(cur_stmt)){
                ast.modify_node(uid, string(opstmt->getOpcodeStr().data()));
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
            if(const UnaryOperator* unary = dyn_cast<UnaryOperator>(cur_stmt)){
                std::string name = UnaryOperator::getOpcodeStr(unary->getOpcode()).data();
                std::string value = ParseHelper::getVarName(unary->getSubExpr()->IgnoreParens()->IgnoreImpCasts());
                if(unary->isPostfix()){
                    name += "_pre";
                } else {
                    name += "_suf";
                }
                ast.modify_node(uid, name);
                ast.add_defination(value);
                ast.add_usage(value);
            }
            if(const CallExpr* call_expr = dyn_cast<CallExpr>(cur_stmt)){
                std::string name = call_expr->getDirectCallee()->getNameAsString();
                ast.add_call(uid, name);
                int sz = call_expr->getNumArgs();
                for(int i = 0; i < sz; i++){
                    const Expr* arg = call_expr->getArg(i);
                    std::string arg_name = ParseHelper::getVarName(arg->IgnoreImpCasts());
                    ast.add_usage(arg_name);
                }
            }
            if(const DeclStmt* ds = dyn_cast<DeclStmt>(cur_stmt)){
                if(ds->isSingleDecl()){
                    const Decl* d = ds->getSingleDecl();
                    if(const ValueDecl* vd = dyn_cast<ValueDecl>(d)){
                        std::string name = vd->getNameAsString();
                        ast.add_defination(name);
                        std::string type = vd->getType().getUnqualifiedType().getCanonicalType().getAsString();
                        ParseHelper::type_simplify(ast, uid, type);
                    }
                }
            }
            if(const DeclRefExpr* dr = dyn_cast<DeclRefExpr>(cur_stmt)){
                const ValueDecl* vd = dr->getDecl();
                if(vd){
                    std::string type = vd->getType().getCanonicalType().getAsString();
                    ParseHelper::type_simplify(ast, uid, type);
                }
            }
            if(const CastExpr* expr = dyn_cast<CastExpr>(cur_stmt)){
                std::string kind = expr->getCastKindName();
                ast.modify_node(uid, kind);
            }
            if(const IntegerLiteral* literal = dyn_cast<IntegerLiteral>(cur_stmt)){
                long long v = *(literal->getValue().getRawData());
                string val = std::to_string(v);
                ast.modify_node(uid, val);
                for(char c: val){
                    ast.add_edge(uid, ast.get_next());
                    ast.add_node(string(c, 1));
                }
            }
            if(const FloatingLiteral* literal = dyn_cast<FloatingLiteral>(cur_stmt)){
                double v = literal->getValue().convertToDouble();
                string val = std::to_string(v);
                ast.modify_node(uid, val);
                for(char c: val){
                    ast.add_edge(uid, ast.get_next());
                    ast.add_node(string(c, 1));
                }
            }
            if(const clang::StringLiteral* literal = dyn_cast<clang::StringLiteral>(cur_stmt)){
                ast.modify_node(uid, "string");
            }
            for(Stmt::child_iterator child = cur_stmt->child_begin(); child != cur_stmt->child_end(); child++){
                if(*child == nullptr){
                    continue;
                }
                string classname = (*child)->getStmtClassName();
                if(classname.length() > 0){
                    int v = ast.get_next();
                    que.push(make_pair(*child, v));
                    ast.add_edge(uid, v);
                    ast.add_node(classname);
                }
            }
        }
        return ast;
    }
}

