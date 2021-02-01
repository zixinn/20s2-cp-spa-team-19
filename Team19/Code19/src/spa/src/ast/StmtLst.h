#include <vector>
#include "Index.fwd.h"
//#include "Node.fwd.h"
//#include "StmtLst.fwd.h"


namespace ast {
	class StmtLst : public Node
	{
		std::vector<Stmt*> statements;
	public:
		StmtLst(Token* token, std::vector<Stmt*> statements) :Node{ token }, statements{ statements} {}
		std::vector<Stmt*> getStatements() { return statements;  };
		~StmtLst() {
			for (int i = 0; i < statements.size(); ++i) {
				delete statements[i];
			}
			statements.clear();
		}
		

		// unable to get iterators to work
		//std::vector<Stmt*>::iterator getIterator() { return statements.begin(); };
	};
}
