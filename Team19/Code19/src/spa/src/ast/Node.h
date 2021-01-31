#pragma once
#include "Index.fwd.h"
#include "sp/Token.h"
//#include "Node.fwd.h"

// stub for actual Token, moved to .fwd.h
//typedef string Token;
//typedef int Index;

namespace ast {
	class Node
	{
		Token* token;
	public:
		Node(Token* token) :token{ token } {};
		inline bool compareToken(Token* tok) { return token->compare(tok); };
		std::string getTokenLiteral() { return const_cast<Token*>(token)->getLiteral(); };
		inline Token* getToken() { return token; };

	};

	class StubNode : public Node
	{
	public:

		StubNode(Token* token) :Node( token ) {}		// contructor

	};

	//Node * Node::makeStub(Token* token) {
	//	return new StubNode(token);//new StubNode(tokenLiteral);
	//}
}
