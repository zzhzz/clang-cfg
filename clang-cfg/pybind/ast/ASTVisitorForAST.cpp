//
// Created by zzhzz on 2019/11/14.
//

#include "ASTVisitorForAST.h"
#include "ParseHelper.h"

#include <queue>

namespace clang_cfg{

    using string = std::string;
    using std::queue;

    bool ASTVisitorForAST::VisitTranslationUnitDecl(clang::TranslationUnitDecl *root) {
        AST ast;
        ast.add_node(string(dyn_cast<Decl>(root)->getDeclKindName()));
        for(DeclContext::decl_iterator it = root->decls_begin(); it != root->decls_end(); it++){
            if(*it == nullptr) continue;
            Decl *decl = *it;
            if(isa<FunctionDecl>(decl)){
                auto* functionDecl = dyn_cast<FunctionDecl>(decl);
                if(!ParseHelper::isInSystem(context, functionDecl) && functionDecl->hasBody()){
                    string function_name = functionDecl->getNameInfo().getAsString(),
                            node_label = string(dyn_cast<Decl>(functionDecl)->getDeclKindName());
                    int function_decl_id = ast.get_next();
                    ast.add_node(node_label);
                    ast.add_edge(0, function_decl_id);
                    int function_name_id = ast.get_next();
                    ast.add_node(function_name);
                    ast.add_edge(function_decl_id, function_name_id);
                    Stmt* functionStmt = functionDecl->getBody();
                    queue<pair<Stmt*, int>> qstmt;
                    qstmt.push(std::make_pair(functionStmt, function_decl_id));
                    while(!qstmt.empty()){
                        int parent = qstmt.front().second;
                        Stmt* root_stmt = qstmt.front().first;
                        string root_label = root_stmt->getStmtClassName();
                        qstmt.pop();
                        int root_id = ast.get_next();
                        ast.add_node(root_label);
                        ast.add_edge(parent, root_id);

                        if(isa<BinaryOperator>(root_stmt)){
                            string op = dyn_cast<BinaryOperator>(root_stmt)->getOpcodeStr().data();
                            int op_id = ast.get_next();
                            ast.add_node(op);
                            ast.add_edge(root_id, op_id);
                        }

                        if(isa<UnaryOperator>(root_stmt)){
                            string op = UnaryOperator::getOpcodeStr(dyn_cast<UnaryOperator>(root_stmt)->getOpcode()).data();
                            int op_id = ast.get_next();
                            ast.add_node(op);
                            ast.add_edge(root_id, op_id);
                        }

                        if(isa<CallExpr>(root_stmt)){
                            auto* callExpr = dyn_cast<CallExpr>(root_stmt);
                            string callee = callExpr->getDirectCallee()->getNameAsString();
                            int call_id = ast.get_next();
                            ast.add_node(callee);
                            ast.add_edge(root_id, call_id);
                            size_t arg_size = callExpr->getNumArgs();
                            for(int idx = 0; idx < arg_size; idx++){
                                const Expr* expr = callExpr->getArg(idx);
                                string arg_name = ParseHelper::getVarName(expr->IgnoreImpCasts());
                                if(!arg_name.empty()){
                                    int arg_id = ast.get_next();
                                    ast.add_node(arg_name);
                                    ast.add_edge(root_id, arg_id);
                                }
                            }
                        }

                        if(isa<DeclStmt>(root_stmt)){
                            auto* decls = dyn_cast<DeclStmt>(root_stmt);
                            if(decls->isSingleDecl()){
                                const Decl* d = decls->getSingleDecl();
                                if(auto* vd = dyn_cast<ValueDecl>(d)){
                                    string var_name = vd->getNameAsString();
                                    int var_id = ast.get_next();
                                    ast.add_node(var_name);
                                    ast.add_edge(root_id, var_id);
                                }
                            }
                        }

                        if(isa<DeclRefExpr>(root_stmt)){
                            auto* declRef = dyn_cast<DeclRefExpr>(root_stmt);
                            string var_name = declRef->getNameInfo().getName().getAsString();
                            int var_id = ast.get_next();
                            ast.add_node(var_name);
                            ast.add_edge(root_id, var_id);
                        }

                        for(Stmt::child_iterator stmt_it = root_stmt->child_begin(); stmt_it != root_stmt->child_end(); stmt_it++){
                            if(*stmt_it == nullptr) continue;
                            string label = (*stmt_it)->getStmtClassName();
                            if(!label.empty()){
                                qstmt.push(std::make_pair(*stmt_it, root_id));
                            }
                        }
                    }

                    for(FunctionDecl::param_iterator param_it = functionDecl->param_begin(); param_it != functionDecl->param_end(); param_it++){
                        ParmVarDecl* parmVarDecl = *param_it;
                        if(parmVarDecl == nullptr) continue;
                        string param_name = parmVarDecl->getDeclKindName();
                        if(!param_name.empty()){
                            int param_id = ast.get_next();
                            ast.add_node(string(parmVarDecl->getDeclKindName()));
                            ast.add_edge(function_decl_id, param_id);
                            int param_name_id = ast.get_next();
                            ast.add_node(param_name);
                            ast.add_edge(param_id, param_name_id);
                        }
                    }
                }
            }
        }
        ASTList::getInst().vecs.push_back(ast);
    }
}
