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
		std::string getTokenLiteral() { return const_cast<sp::Token*>(token)->getLiteral(); };
		inline sp::Token* getToken() { return token; };

		inline bool compareToken(sp::Token* tok) { 
			if (!tok) { throw "Node::compareToken other token is nullptr"; }
			return token->compare(tok); 
		};

		~Node() { delete token; }

	};

	class StubNode : public Node
	{
	public:

		StubNode(sp::Token* token) :Node( token ) {}		// contructor

	};
}
