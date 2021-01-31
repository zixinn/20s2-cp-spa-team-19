//  VarName
#include "Index.fwd.h"
#include "sp/Token.h"
//#include "Node.fwd.h"
//#include "Expr.fwd.h"
//#include "VarName.fwd.h"

namespace ast {
	class VarName : public Expr
	{
	public:
		const std::string val;		// to change to STRING typedef after integration

		VarName(Token* token, std::string val) :Expr{ token }, val{ val } {}
		//std::string getTokenLiteral() override { return const_cast<Token*>(token)->getLiteral(); };
		//inline Token* getToken() override { return token; };
		bool compare(Expr* expr) override {
			if (!expr) { throw "compare against null"; }
			bool sameTok = Expr::compareToken(expr->getToken());
			if (!sameTok) { return false; }

			bool sameType = typeid(this) == typeid(expr);
			if (!sameType) { return false; }

			bool valCheck = this->val == ((VarName*)expr)->val;
			return valCheck;
		};

		std::string toString() override {
			return this->val;
		};

		std::string getVal() { return val; }


	};
}
