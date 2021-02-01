#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//#include "PKB/PKB.h"
//#include "TNode.h"

#include "ast/Index.h" // need this so assignStmt knows stmt exist
#include "SP/Parser.h"

namespace TokenUtils {

}

Parser::Parser(LexerStub* l_ptr) : l_ptr{l_ptr} {
	this->nextToken();
	this->nextToken();
}

void Parser::nextToken() {
	currToken = peekToken;
	peekToken = (*l_ptr).nextToken();
};

ast::StmtLst* Parser::parseStmtLst() {
	std::vector<ast::Stmt*> xs{};
	while (this->currToken && (!this->currTokenIs(Token::TokenType::EOFF))) {
		ast::Stmt* stmt = this->parseStmt();
		if (!stmt) { throw this->genError("ParseStmtLst error"); }
		xs.push_back(stmt);
		// currToken is semicolon
		// shuold i also check for } ?
		if (!this->currTokenIs(Token::TokenType::SEMICOLON)) { 
			throw this->genError("ParseStmtLst expected Semicolon, got: " + this->currLiteral());
		}
		this->nextToken();		
	}
	ast::StmtLst* stmt_lst = new ast::StmtLst(new Token(Token::TokenType::LBRACE, "PLACEHOLDER"), xs);
	return stmt_lst;
}

ast::Stmt* Parser::parseStmt() {
	if (this->currTokenIs(Token::TokenType::NAME)) {
		return this->parseAssignStmt();
	} else if (this->currTokenIs(Token::TokenType::CALL)) {
		return this->parseCallStmt();
	}
	throw this->genError("ParseStmt received unexpected token: " + this->currLiteral());
}



ast::CallStmt* Parser::parseCallStmt() {
	if (!this->currTokenIs(Token::TokenType::CALL)) {
		throw this->genError("ParseCall expected a CALL, got: " + this->currLiteral());
	}
	Token* call_tok = this->currToken;
	
	if (!this->expectPeek(Token::TokenType::NAME)) {
		throw this->genError("ParseCall expected a NAME, got: " + this->peekLiteral()); //peek adv only when tru
	}
	ast::ProcName* pn = this->parseProcName();

	if (!this->expectPeek(Token::TokenType::SEMICOLON)) {
		throw this->genError("ParseCall expected a SEMICOLON, got: " + this->peekLiteral()); //peek adv only when tru
	}

	return new ast::CallStmt(this->getPlusPC(), call_tok, pn);
}
//ast::ReadStmt* Parser::parseReadStmt() {
//	throw "not ready";
//}
//ast::PrintStmt* Parser::parsePrintStmt() {
//	throw "not ready";
//}


ast::AssignStmt* Parser::parseAssignStmt() {
	if (!this->currTokenIs(Token::TokenType::NAME)) {
		throw this->genError("ParseAssign expected a NAME, got: " + this->currLiteral());
	}
	ast::VarName* vn = this->parseVarName();

	if (!this->expectPeek(Token::TokenType::ASSIGN)) {
		throw this->genError("ParseAssign expected a ASSIGN, got: " + this->peekLiteral()); //peek adv only when tru
	}
	Token* assToken = this->getCurrToken();

	while (!this->currTokenIs(Token::TokenType::SEMICOLON)) {
		this->nextToken();
	}

	ast::AssignStmt* ass = new ast::AssignStmt(this->getPlusPC(), assToken, vn, nullptr); // expr not calc yet
	// when returning, currToken must be semicolon
	return ass;
}

std::string Parser::genError(std::string str) {
	return "StmtNum: " + std::to_string(this->pc) + " - " + str;
}

int Parser::getPlusPC() {
	int out = this->pc;
	this->pc++;
	return out;
}

ast::VarName* Parser::parseVarName() {
	Token* tok = this->currToken;
	if (currTokenIs(Token::TokenType::NAME)) {
		return new ast::VarName(tok, tok->getLiteral());
	}
	throw this->genError("ParseVarName expected a NAME, got: " + tok->getLiteral());
}

ast::ProcName* Parser::parseProcName() {
	Token* tok = this->currToken;
	if (currTokenIs(Token::TokenType::NAME)) {
		return new ast::ProcName(tok, tok->getLiteral());
	}
	throw this->genError("ParseProcName expected a NAME, got: " + tok->getLiteral());
}

std::string Parser::peekLiteral() {
	if (!this->peekToken) {
		return "nullptr:??";
	}
	return this->peekToken->getLiteral();
}

std::string Parser::currLiteral() {
	if (!this->currToken) {
		return "nullptr:??";
	}
	return this->currToken->getLiteral();
}

bool Parser::currTokenIs(Token::TokenType tok_type) {
	if (!this->currToken) {
		throw this->genError("currToken Error");
	}
	return this->currToken->getType() == tok_type;
}

bool Parser::peekTokenIs(Token::TokenType tok_type) {
	if (!this->peekToken) {
		throw this->genError("peekToken Error");
	}
	return this->peekToken->getType() == tok_type;
}

bool Parser::expectPeek(Token::TokenType tok_type) {
	if (this->peekTokenIs(tok_type)) {
		this->nextToken();
		return true;
	}
	return false;
}

bool Parser::parseTest() {
	return false;
}

Token* LexerStub::nextToken() {
	if (index >= this->tokens.size()) {
		int diff = index - this->tokens.size() + 1;
		return new Token(Token::TokenType::EOFF, "EOFF Count: " + std::to_string(diff)); //EOF
	}
	Token* out = this->tokens[index];
	++index;
	return out;
}
