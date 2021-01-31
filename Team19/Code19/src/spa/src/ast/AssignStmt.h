#include "Index.fwd.h"

/*
*  ast/Index.fwd.h contains forward declaration sp/Token.fwd.h
*  ast/AssignStmt.h needs to know sp/Token.h before it can work
*/
#include "sp/Token.h"
//#include "Node.fwd.h"
//#include "Expr.fwd.h"
//#include "Stmt.fwd.h"
//#include "VarName.fwd.h"
//#include "AssignStmt.fwd.h"

// Assign
namespace ast {
	class AssignStmt : public Stmt {
		VarName* var_name;
		Expr* expr;
	public:
	//	AssignStmt(int stmtNum, Token token, VarName* var_name, Expr* expr);
		AssignStmt(int stmtNum, Token* token, VarName* var_name, Expr* expr)
			:Stmt( token, stmtNum ), var_name{ var_name }, expr{ expr } {}
		VarName* getName() { return var_name; }
		Expr* getExpr() { return expr; }
		bool compareExpr(Expr* otherExpr) { return this->expr->compare(otherExpr); }

		~AssignStmt() {
			delete var_name;
			delete expr;
		}

	};
}
