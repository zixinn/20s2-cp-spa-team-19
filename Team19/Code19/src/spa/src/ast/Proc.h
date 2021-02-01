// Procedure 
#include "Index.fwd.h"
#include "SP/Token.h"

namespace ast {
	class Proc : public Node
	{

		ast::ProcName* proc_name;
		ast::StmtLst* stmt_lst;
	public:

		Proc(Token* token, ast::ProcName* proc_name, ast::StmtLst* stmt_lst) 
			:Node{ token }, proc_name{ proc_name }, stmt_lst{ stmt_lst } {}		// contructor

	};
}
