#include "main.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/GraphWriter.h"
// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.

using namespace llvm;

void CallGraphNode::gen_CFG(std::ofstream& fout, ASTContext& TheContext){
	Stmt* funcbody = this->FD->getBody();
	std::map<std::pair<int,int>,int> g;
	g.clear();
	std::unique_ptr<CFG> sourcecfg = CFG::buildCFG(this->FD, funcbody, &TheContext, CFG::BuildOptions());
	CFG* cfg = sourcecfg.get();
	fout << cfg->size() << std::endl;
	for(CFG::const_iterator i = cfg->begin(); i != cfg->end(); i++){
		int u = (*i)->getBlockID();
		fout << (*i)->size() << std::endl;
		for(CFGBlock::const_iterator it = (*i)->begin(); it != (*i)->end(); it++){
			Optional<CFGStmt> stmt = it->getAs<CFGStmt>();
			if(stmt){
				this->out(const_cast<Stmt*>(stmt->getStmt()), fout, TheContext);
			} else {
				fout << 0 << std::endl;
				fout << 0 << std::endl;
				fout << 0 << std::endl;
				fout << 0 << std::endl;
			}
		}
		if(Stmt* s = (*i)->getTerminatorCondition()){
			fout << 1 << std::endl;
			this->out(s, fout, TheContext);
		} else {
			fout << 0 << std::endl;
		}
		int type = 0;
		int sz = (*i)->succ_size();
		if(sz == 2){
			type = 1;
		} else if(sz == 1){
			type = 0;	
		} else if(sz > 2){
			type = 5;
		}
		for(CFGBlock::succ_iterator it = (*i)->succ_begin(); it != (*i)->succ_end(); it++){
			CFGBlock* block = *it;
			if(block == NULL)
				continue;
			int v = block->getBlockID();
			std::pair<int,int> edge = std::make_pair(u, v);
			if(g.find(edge) == g.end()){
				g[edge] = type;
				type += 1;
			}
		}
	}
	fout << g.size() << std::endl; 
	for(std::map<std::pair<int,int>,int>::iterator it = g.begin(); it != g.end(); it++){
		fout << it->first.first << ' ' << it->first.second <<  ' ' << it->second << std::endl;
	}
	fout << -1 << std::endl;

}

std::string get_name(const Expr *expr){
	std::queue<const Expr*> q;
	q.push(expr);
	while(!q.empty()){
		const Expr* t = q.front();
		// std::cout << t->getStmtClassName() << std::endl;
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

void CallGraphNode::out(Stmt *s, std::ofstream& fout, ASTContext& ctx){
	std::vector<std::string> nodes;
	std::vector<std::pair<int,int> > ast;
	std::vector<std::pair<int,std::string> > calling;
	std::set<std::string> define, use;
	define.clear();
	use.clear();
	nodes.push_back(s->getStmtClassName());
	std::queue<std::pair<Stmt*, int> > q = std::queue<std::pair<Stmt*, int> >();
	q.push(std::make_pair(s, 0));
	while(!q.empty()){
		Stmt* t = q.front().first;
		int u = q.front().second;
		q.pop();
		if(const BinaryOperator * b = dyn_cast<BinaryOperator>(t)){
			ast.push_back(std::make_pair(u, nodes.size()));
			nodes.push_back(b->getOpcodeStr().data());
			std::string lvalue = get_name(b->getLHS()->IgnoreParens()->IgnoreImpCasts());
			std::string rvalue = get_name(b->getRHS()->IgnoreParens()->IgnoreImpCasts());

			if(b->isAssignmentOp() || b->isCompoundAssignmentOp() || b->isShiftAssignOp()){
				if(!lvalue.empty()){
					define.insert(lvalue);
				}
				if(!rvalue.empty()){
					use.insert(rvalue);
				}
			} else {
				if(!lvalue.empty()){
					use.insert(lvalue);
				}
				if(!rvalue.empty()){
					use.insert(rvalue);
				}
			}
		}
		if(const UnaryOperator * unary = dyn_cast<UnaryOperator>(t)){
			std::string name = UnaryOperator::getOpcodeStr(unary->getOpcode()).data();
			std::string value = get_name(unary->getSubExpr()->IgnoreParens()->IgnoreImpCasts());
			if(unary->isPostfix()){
				name += "_pre";
			} else {
				name += "_suf";
			}
			ast.push_back(std::make_pair(u, nodes.size()));
			nodes.push_back(name);
			// std::cout << "unary " + value << std::endl;
			if(!value.empty()){
				define.insert(value);
				use.insert(value);
			}
		}
		if(const CallExpr * c = dyn_cast<CallExpr>(t)){
			std::string name = c->getDirectCallee()->getNameAsString();
			for(int i = 0; i < name.length(); i++){
				if(name[i] == ' '){
					name[i] = '_';
				}
			}
			calling.push_back(std::make_pair(u,name));
			int sz = c->getNumArgs();
			for(int i = 0; i < sz; i++){
				const Expr* arg = c->getArg(i);
				std::string arg_name = get_name(arg->IgnoreImpCasts());
				if(!arg_name.empty()){
					use.insert(arg_name);
				}
			}
		}
		if(const DeclStmt * ds = dyn_cast<DeclStmt>(t)){
			if(ds->isSingleDecl()){
				const Decl* d = ds->getSingleDecl();
				if(const ValueDecl * vd = dyn_cast<ValueDecl>(d)){
					std::string name = vd->getNameAsString();
					if(!name.empty()){
						define.insert(name);
					}
					std::string type = vd->getType().getUnqualifiedType().getCanonicalType().getAsString();
					for(int i = 0; i < type.length(); i++){
						if(type[i] == ' '){
							type[i] = '_';
						}
					}
					if(type.length() > 0){
						ast.push_back(std::make_pair(u, nodes.size()));
						nodes.push_back(type);
					}
				}
			}
		}
		if(const DeclRefExpr * dr = dyn_cast<DeclRefExpr>(t)){
			if(const ValueDecl * d = dr->getDecl()){
				std::string type = d->getType().getCanonicalType().getAsString();
				for(int i = 0; i < type.length(); i++){
					if(type[i] == ' '){
						type[i] = '_';
					}
				}
				if(type.length() > 0){
					ast.push_back(std::make_pair(u, nodes.size()));
					nodes.push_back(type);
				}
			}
		}
		if(const ImplicitCastExpr * expr = dyn_cast<ImplicitCastExpr>(t)){
			std::string kind = expr->getCastKindName();
			ast.push_back(std::make_pair(u, nodes.size()));
			nodes.push_back(kind);
		}
		if(isa<IntegerLiteral>(t)){
			const IntegerLiteral * literal = dyn_cast<IntegerLiteral>(t);
			long long  v = *literal->getValue().getRawData();
			nodes[u] = "const_" + std::to_string(v);
		}
		for(Stmt::child_iterator it = t->child_begin(); it != t->child_end(); it++){
			if(*it == NULL)
				continue;
			std::string name = (*it)->getStmtClassName();
			if(name.length() > 0) {
				q.push(std::make_pair(*it, nodes.size()));
				ast.push_back(std::make_pair(u, nodes.size()));
				nodes.push_back(name);
			}
		}
	}
	fout << define.size() << std::endl;
	for(std::set<std::string>::iterator it = define.begin(); it != define.end(); it++){
		fout << *it << std::endl;
	}
	fout << use.size() << std::endl;
	for(std::set<std::string>::iterator it = use.begin(); it != use.end(); it++){
		fout << *it << std::endl;
	}
	fout << nodes.size() << std::endl;
	for(int i = 0; i < nodes.size(); i++){
		for(int j = 0; j < nodes[i].length(); j++){
			if(nodes[i][j] == ' '){
				nodes[i][j] = '_';
			}
		}
		fout << nodes[i] << ' ';
	}
	fout << std::endl;

	for(int i = 0; i < ast.size(); i++){
		fout << ast[i].first << ' ' << ast[i].second << std::endl;
	}
	fout << calling.size() << std::endl;
	for(int i = 0; i < calling.size(); i++){
		fout << calling[i].first << ' ' << calling[i].second << std::endl;
	}
}


class CallGraph : public RecursiveASTVisitor<CallGraph> {
	public:
		CallGraph(ASTContext &C, Rewriter &R) : TheContext(C), TheRewriter(R) { idx = 0; }

		bool VisitStmt(Stmt *s) {
			return true;
		}

		bool isInSystem(Decl *decl) {
			if (TheContext.getSourceManager().isInSystemHeader(decl->getLocation()) ||
					TheContext.getSourceManager().isInExternCSystemHeader(decl->getLocation()) ||
					TheContext.getSourceManager().isInSystemMacro(decl->getLocation())) {
				return true;
			}
			return false;
		}

		void addToCallGraph(Decl *d){
			TraverseDecl(d);
		}

		bool canIncludeInGraph(const Decl *d){
			if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(d)) {
				if(FD->isDependentContext()){
					return false;
				}
				IdentifierInfo *II = FD->getIdentifier();
				if(II && II->getName().startswith("__inline")){
					return false;
				}
			}
			return true;
		}

		bool canBeCallerInGraph(const Decl *d){
			if(!d->hasBody()){
				return false;
			}
			if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(d)) {
				if(FD->isDependentContext()){
					return false;
				}
				IdentifierInfo *II = FD->getIdentifier();
				if(II && II->getName().startswith("__inline")){
					return false;
				}
			}
			return true;
		}


		void get_filename(std::string& path){
			std::string tmp;
			for(int i = path.length()-1; i >= 0; i--){
				if(path[i] == '/') break;
				tmp += path[i];
			}
			std::reverse(tmp.begin(), tmp.end());
			path = tmp;
		}

		void addRootNode(Decl *d, std::ofstream& fout){
			d = d->getCanonicalDecl();
			CallGraphNode* root = getOrInsertNode(d);
			fout << root->getNameAsString() << std::endl;
			root->gen_CFG(fout, TheContext);
			CGBuilder builder(this, root, TheContext);
			if(Stmt *body = d->getBody()){
				builder.Visit(body);
			}
		}

		CallGraphNode* getNode(Decl *d){
			iterator it = Roots.find(d);
			if (it == Roots.end()) {
				return nullptr;
			}
			return it->second.get();
		}

		CallGraphNode* getOrInsertNode(Decl *d){
			d = d->getCanonicalDecl();
			std::shared_ptr<CallGraphNode> &Node = Roots[d];
			if(!Node){
				Node = std::shared_ptr<CallGraphNode>(new CallGraphNode(d));
				hash[d] = (this->idx)++;
			}
			return Node.get();
		}

		bool VisitFunctionDecl(FunctionDecl *f) {
			// Only function definitions (with bodies), not declarations.
			if (f->hasBody()) {
				if(isInSystem(f)){
					return true;
				}
				Stmt* funcBody = f->getBody();
				// funcBody->dump()
				std::string fname = this->TheContext.getSourceManager().getFilename(funcBody->getBeginLoc()).str();
				std::cout << "In: " << fname << std::endl;
				get_filename(fname);
				this->filename = fname;
				if(canBeCallerInGraph(f) && f->isThisDeclarationADefinition()){
					fout = std::ofstream(filename + ".cfg.txt", std::ios::app);
					addRootNode(f, fout);
					fout.close();
				}
			}
			return true;
		}

	private:
		ASTContext &TheContext;
		Rewriter &TheRewriter;
		std::ofstream fout;
		std::string filename;
		typedef llvm::DenseMap<const Decl *, std::shared_ptr<CallGraphNode>> RootsMapType;
		typedef RootsMapType::iterator iterator;
		typedef RootsMapType::const_iterator const_iterator;
		RootsMapType Roots;
		int idx;
		std::map<const Decl *, int> hash;
};

std::string CallGraphNode::getNameAsString() const {

	if (FunctionDecl *decl = llvm::dyn_cast_or_null<FunctionDecl>(FD)) {

		return decl->getNameAsString();
	} else if (ObjCMethodDecl *decl = llvm::dyn_cast_or_null<ObjCMethodDecl>(FD)) {

		return decl->getNameAsString();
	} else {

		return "";
	}
}

void CGBuilder::VisitStmt(Stmt *S) {
	VisitChildren(S);
}

void CGBuilder::VisitCallExpr(CallExpr *CE) {
	if (Decl *D = getDeclFromCall(CE)) {
		if (G->isInSystem(D)) {
			return;
		}
		addCalledDecl(D);
	}
	VisitChildren(CE);
}

Decl* CGBuilder::getDeclFromCall(CallExpr *CE) {

	if (FunctionDecl *CalleeDecl = CE->getDirectCallee())
		return CalleeDecl;

	// Simple detection of a call through a block.
	Expr *CEE = CE->getCallee()->IgnoreParenImpCasts();
	if (BlockExpr *Block = dyn_cast<BlockExpr>(CEE)) {

		return Block->getBlockDecl();
	}

	return nullptr;
}
	
void CGBuilder::addCalledDecl(Decl *D) {
	if (G->canIncludeInGraph(D)) {
		CallGraphNode *CalleeNode = G->getOrInsertNode(D);
		CallerNode->addCallee(CalleeNode);
	}
}

void CGBuilder::VisitChildren(Stmt *S) {

	for (Stmt *SubStmt : S->children())
		if (SubStmt)
			this->Visit(SubStmt);
}
	
// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
	public:
		MyASTConsumer(ASTContext &C, Rewriter &R) : Visitor(C, R) {}

		void HandleTranslationUnit(ASTContext& context){
			Visitor.addToCallGraph(context.getTranslationUnitDecl());
		}

	private:
		CallGraph Visitor;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
	public:
		MyFrontendAction() {}

		std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
			TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
			return llvm::make_unique<MyASTConsumer>(CI.getASTContext(), TheRewriter);
		}

	private:
		Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, ToolingSampleCategory);
	ClangTool Tool(op.getCompilations(), op.getSourcePathList());

	return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}

