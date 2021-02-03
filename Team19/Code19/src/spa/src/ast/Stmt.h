#include "Index.fwd.h"
#include "SP/Token.h"
#include "Node.h"
//#include "Node.fwd.h"
//#include "Stmt.fwd.h"

// Interface Stmt for anything with a statement number
namespace ast {
	class Stmt : public Node
	{
		Index index;
	public:
		Stmt(Token* token, Index index) :Node{ token }, index{ index } {};
		Index getIndex() { return index; };
	};
}
