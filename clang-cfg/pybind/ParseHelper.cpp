//
// Created by zzhzz on 2019-09-26.
//

#include <queue>

#include "extend_cfg/global.h"
#include "ParseHelper.h"

namespace clang_cfg{
    using namespace clang;
    using namespace llvm;

    bool ParseHelper::isInSystem(ASTContext& context, const FunctionDecl* decl) {
        if(decl->isInStdNamespace() || decl->isDependentContext()) {
            return true;
        }
        if(decl->getLocation().isValid()){
            if(context.getSourceManager().isInSystemHeader(decl->getLocation())) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    bool ParseHelper::canIncludeInGraph(Decl* decl) {
        if(isa<FunctionDecl>(decl)){
            const FunctionDecl* functionDecl = dyn_cast<FunctionDecl>(decl);
            if(functionDecl->isDependentContext()) {
                return false;
            }
            IdentifierInfo* identifierInfo = functionDecl->getIdentifier();
            if(identifierInfo && identifierInfo->getName().startswith("__inline")) {
                return false;
            }
        }
        return true;
    }

    bool ParseHelper::canBeCallerInGraph(Decl* decl) {
        if(!decl->hasBody()) {
            return false;
        }
        return canIncludeInGraph(decl);
    }

    string ParseHelper::getVarName(const Expr* expr) {
        std::queue<const Expr*> q;
        q.push(expr);
        while(!q.empty()){
            const Expr* t = q.front();
            q.pop();
            if(isa<DeclRefExpr>(t)){
                const DeclRefExpr* decl = dyn_cast<DeclRefExpr>(t);
                return decl->getNameInfo().getAsString();
            }
            if(isa<ArraySubscriptExpr>(t)){
                const ArraySubscriptExpr* array = dyn_cast<ArraySubscriptExpr>(t);
                q.push(array->getBase()->IgnoreImpCasts());
            }
            if(isa<MemberExpr>(t)){
                const MemberExpr* mem_expr = dyn_cast<MemberExpr>(t);
                q.push(mem_expr->getBase()->IgnoreImpCasts());
            }
        }
        return "";

    }

    Decl* ParseHelper::getDeclFromCall(clang::CallExpr *callExpr) {
        FunctionDecl* functionDecl = callExpr->getDirectCallee();
        if(functionDecl){
            return functionDecl;
        }
        Expr* call_expr = callExpr->getCallee()->IgnoreParenCasts();
        if(isa<BlockExpr>(call_expr)){
            BlockExpr* blockExpr = dyn_cast<BlockExpr>(call_expr);
            return blockExpr->getBlockDecl();
        }
        return nullptr;
    }

    void replace_all(string& type, const string& pattern, const string& target) {
        int pos = -1;
        while((pos = type.find(pattern)) != string::npos){
            type.replace(pos, pattern.length(), target);
        }
    }

    void ParseHelper::type_simplify(AST &ast, int uid, string &type) {
        set<string> type_set;
        type_set.insert({"int", "float", "double", "long", "char", "struct", "class", "string"});
        if(type_set.find(type) != type_set.end()){
            ast.modify_node(uid, type);
            return ;
        }
        replace_all(type, "long long", "long");
        replace_all(type, "long double", "double");
        replace_all(type, "unsigned", "^");
        replace_all(type, "const", "=");
        replace_all(type, "struct", "$");
        replace_all(type, "std", "+");
        replace_all(type, "class", "@");
        replace_all(type, "int", "~");
        replace_all(type, "float", "-");
        replace_all(type, "double", "!");
        replace_all(type, "long", "#");
        replace_all(type, "char", "%");
        replace_all(type, "string", ";");
        replace_all(type, "::", ":");
        replace_all(type, " ", "");
        for(char c: type) {
            if(!isalpha(c)) {
                ast.add_edge(uid, ast.get_next());
                switch(c){
                    case '.': ast.add_node("std"); break;
                    case '$': ast.add_node("struct"); break;
                    case '^': ast.add_node("unsigned"); break;
                    case '=': ast.add_node("const"); break;
                    case '@': ast.add_node("class"); break;
                    case '~': ast.add_node("int"); break;
                    case '-': ast.add_node("float"); break;
                    case '!': ast.add_node("double"); break;
                    case '#': ast.add_node("long"); break;
                    case '%': ast.add_node("char"); break;
                    case ';': ast.add_node("string"); break;
                    default: ast.add_node(string(1, c));
                }
            }
        }
    }

}


