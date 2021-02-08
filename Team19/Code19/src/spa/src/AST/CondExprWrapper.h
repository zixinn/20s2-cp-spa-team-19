#pragma once
#include <string>
#include "SP/Token.h"
#include "AST/CondExpr.h"

namespace ast {
	class CondExprWrapper : public CondExpr {
		Expr* inside;
	public:
		CondExprWrapper(sp::Token* token, Expr* inside)
			:CondExpr(token), inside{ inside } {}
		Expr* unWrap() { return inside; }

		std::string toString() override {
			std::string inner_str = inside ? inside->toString() : "[NULL_PTR]";
			return inner_str;
		}

		~CondExprWrapper() {
			delete inside;
		}

	};
}
