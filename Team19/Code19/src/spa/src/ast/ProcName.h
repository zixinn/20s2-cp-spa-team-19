// ProcNames are callable
#include "Index.fwd.h"
#include "sp/Token.h"

//#include "Node.fwd.h"
//#include "Expr.fwd.h"
//#include "ProcName.fwd.h"

namespace ast {
	class ProcName : public Expr
	{
	public:
		const std::string val;		// to change to STRING typedef after integration

		ProcName(Token* token, std::string val) :Expr { token }, val{ val } {}

		bool compare(Expr* expr) override {
			return false; // shouldnt have to compare procedure names
		};

		std::string toString() override {
			return this->val;
		};

		std::string getVal() { return val; }
	};
}
