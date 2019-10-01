#ifndef MAIN_H_
#define MAIN_H_
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <queue>
#include <vector>
#include <unistd.h>
#include "clang/Basic/LangOptions.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Analysis/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SetVector.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/Casting.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

class CallGraphNode{
public:
	typedef CallGraphNode* CallRecord;

private:
	Decl* FD;
	SmallVector<CallRecord, 5> CalledFunctions;

public:
	CallGraphNode(Decl *d) : FD(d){}
	typedef SmallVectorImpl<CallRecord>::iterator iterator;
	typedef SmallVectorImpl<CallRecord>::const_iterator const_iterator;

	void gen_CFG(std::ofstream& fout, ASTContext& TheContext);
	void out(Stmt *s, std::ofstream& fout, ASTContext& ctx);

	inline iterator begin() {
		return CalledFunctions.begin();
	}
	inline iterator end() {
		return CalledFunctions.end(); 
	}
	inline const_iterator begin() const {
		return CalledFunctions.begin();
	}
	inline const_iterator end()   const {
		return CalledFunctions.end();
	}

	bool operator==(const CallGraphNode &node) {
		return this->getDecl() == node.getDecl();
	}

	bool operator!=(const CallGraphNode &node) {
		return this->getDecl() != node.getDecl();
	}

	inline bool empty() const {
		return CalledFunctions.empty();
	}

	inline unsigned size() const {
		return CalledFunctions.size();
	}

	void addCallee(CallGraphNode *N) {
		if (std::find(CalledFunctions.begin(), CalledFunctions.end(), N) != CalledFunctions.end()) {
			return;
		}
		CalledFunctions.push_back(N);
	}

	Decl *getDecl() const {
		return FD; }

	std::string getNameAsString() const;
};

class CallGraph;
class CGBuilder: public StmtVisitor<CGBuilder>{
	CallGraph *G;
	CallGraphNode *CallerNode;
	ASTContext &Context;
	public:
	CGBuilder(CallGraph *g, CallGraphNode *N, ASTContext &context)
		: G(g), CallerNode(N), Context(context) {}
	void VisitStmt(Stmt *s);
	void VisitCallExpr(CallExpr *CE);
	Decl *getDeclFromCall(CallExpr *CE);
	void addCalledDecl(Decl *D);
	 void VisitChildren(Stmt *S);
};

#endif
