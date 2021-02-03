#pragma once
#include "SP/Token.h"

// stub for actual Token, moved to .fwd.h
//typedef string Token;
//typedef int Index;

namespace ast {
	class Node
	{
		sp::Token* token;
	public:
		Node(sp::Token* token) :token{ token } {};
		inline bool compareToken(sp::Token* tok) { return token->compare(tok); };
		std::string getTokenLiteral() { return const_cast<sp::Token*>(token)->getLiteral(); };
		inline sp::Token* getToken() { return token; };

	};

	class StubNode : public Node
	{
	public:

		StubNode(sp::Token* token) :Node( token ) {}		// contructor

	};
}
