#pragma once

/*
 * Purpose: Provide an aggregated forward declaration for all classes in /ast namespace
 * All .h files that need to use another ast .h file need only #include "ast/Index.fwd.h"
 * Contents of .fwd.h files can be repeated multiple seems
 * .fwd.h files are #included by .h files which are #included by .cpp files i assume
*/

#include <string>
#include <vector>
#include "sp/Token.fwd.h"

// stub for actual Token
//typedef std::string Token;
typedef int Index;


namespace ast {
	class AssignStmt;
	class CallStmt;
	class CondExpr;
	class Expr;
	class Node;
	class Proc;
	class ProcName;
	class Program;
	class Stmt;
	class StmtLst;
	class VarName;
}
