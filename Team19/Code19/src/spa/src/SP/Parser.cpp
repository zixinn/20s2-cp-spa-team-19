#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;
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


ast::Program* Parser::parseProgram() {
	std::vector<ast::Proc*> procs{};
	while (this->currToken && (!this->currTokenIs(sp::Token::TokenType::EOFF))) {
		ast::Proc* proc = this->parseProc();
		if (!proc) { throw this->genError("ParseProc error"); }
		procs.push_back(proc);
		// currToken will be } ? 
		// depend on parseStmtLst
		if (!this->currTokenIs(sp::Token::TokenType::LBRACE)) {
			throw this->genError("ParseProc expected LBRACE, got: " + this->currLiteral());
		}
		this->nextToken();
	}
	if (procs.size() == 0) {
		throw this->genError("Expect at least one procedure");
		return new ast::Program(nullptr, procs);
	} else {
		return new ast::Program(procs[0], procs);
	}
}

ast::Proc* Parser::parseProc() {
	if (!this->currTokenIs(sp::Token::TokenType::PROC)) {
		throw this->genError("ParseProc expected a PROC, got: " + this->currLiteral());
	}
	sp::Token* proc_tok = this->currToken;

	if (!this->expectPeekIsNameOrKeyword()) {
		throw this->genError("ParseProc expected a NAME or Keyword, got: " + this->peekLiteral());
	}
	ast::ProcName* pn = this->parseProcName();

	if (!this->expectPeek(sp::Token::TokenType::LBRACE)) {
		throw this->genError("ParseProc expected a LBRACE, got: " + this->peekLiteral()); 
	}

	//current token is {
	this->nextToken();

	ast::StmtLst* stmtlst = this->parseStmtLst();
	return new ast::Proc(proc_tok, pn, stmtlst);
}

ast::StmtLst* Parser::parseStmtLst() {
	std::vector<ast::Stmt*> xs{};
	while (this->currToken && (!this->currTokenIs(sp::Token::TokenType::EOFF))) {
		ast::Stmt* stmt = this->parseStmt();
		if (!stmt) { throw this->genError("ParseStmtLst error"); }
		xs.push_back(stmt);
		// currToken is semicolon
		// shuold i also check for } ?
		if (!this->currTokenIs(sp::Token::TokenType::SEMICOLON)) { 
			throw this->genError("ParseStmtLst expected Semicolon, got: " + this->currLiteral());
		}
		this->nextToken();		
	}
	ast::StmtLst* stmt_lst = new ast::StmtLst(new sp::Token(sp::Token::TokenType::LBRACE, "PLACEHOLDER"), xs);
	return stmt_lst;
}

// NOTE: PROC is not expected here and should be parsed as NAME
ast::Stmt* Parser::parseStmt() {
	if (this->currTokenIs(sp::Token::TokenType::NAME)) {
		return this->parseAssignStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::CALL)) {
		return this->parseCallStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::PRINT)) {
		return this->parsePrintStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::READ)) {
		return this->parseReadStmt();
	}
	throw this->genError("ParseStmt received unexpected token: " + this->currLiteral());
}



ast::CallStmt* Parser::parseCallStmt() {
	if (!this->currTokenIs(sp::Token::TokenType::CALL)) {
		throw this->genError("ParseCall expected a CALL, got: " + this->currLiteral());
	}
	sp::Token* call_tok = this->currToken;
	
	if (!this->expectPeekIsNameOrKeyword()) {
		throw this->genError("ParseCall expected a NAME or Keyword, got: " + this->peekLiteral());
	}
	ast::ProcName* pn = this->parseProcName();

	if (!this->expectPeek(sp::Token::TokenType::SEMICOLON)) {
		throw this->genError("ParseCall expected a SEMICOLON, got: " + this->peekLiteral()); //peek adv only when tru
	}

	return new ast::CallStmt(this->getPlusPC(), call_tok, pn);
}

ast::ReadStmt* Parser::parseReadStmt() {
	if (!this->currTokenIs(sp::Token::TokenType::READ)) {
		throw this->genError("ParseRead expected a READ, got: " + this->currLiteral());
	}
	sp::Token* read_tok = this->currToken;
	
	if (!this->expectPeekIsNameOrKeyword()) {
		throw this->genError("ParseRead expected a NAME or Keyword, got: " + this->peekLiteral());
	}
	ast::VarName* vn = this->parseVarName();

	if (!this->expectPeek(sp::Token::TokenType::SEMICOLON)) {
		throw this->genError("ParseRead expected a SEMICOLON, got: " + this->peekLiteral()); //peek adv only when tru
	}

	return new ast::ReadStmt(this->getPlusPC(), read_tok, vn);
}

ast::PrintStmt* Parser::parsePrintStmt() {
	if (!this->currTokenIs(sp::Token::TokenType::PRINT)) {
		throw this->genError("ParsePrint expected a PRINT, got: " + this->currLiteral());
	}
	sp::Token* print_tok = this->currToken;
	
	if (!this->expectPeekIsNameOrKeyword()) {
		throw this->genError("ParsePrint expected a NAME or Keyword, got: " + this->peekLiteral());
	}
	ast::VarName* vn = this->parseVarName();

	if (!this->expectPeek(sp::Token::TokenType::SEMICOLON)) {
		throw this->genError("ParsePrint expected a SEMICOLON, got: " + this->peekLiteral()); //peek adv only when tru
	}

	return new ast::PrintStmt(this->getPlusPC(), print_tok, vn);
}


ast::AssignStmt* Parser::parseAssignStmt() {
	if (!this->currTokenIsNameOrKeyword()) {
		throw this->genError("ParseAssign expected a NAME or Keyword, got: " + this->currLiteral());
	}
	ast::VarName* vn = this->parseVarName();

	if (!this->expectPeek(sp::Token::TokenType::ASSIGN)) {
		throw this->genError("ParseAssign expected a ASSIGN, got: " + this->peekLiteral()); //peek adv only when tru
	}
	sp::Token* assToken = this->getCurrToken();

	while (!this->currTokenIs(sp::Token::TokenType::SEMICOLON)) {
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
	sp::Token* tok = this->currToken;
	if (currTokenIsNameOrKeyword()) {
		// correct TokenType since original TokenType could be NAME or a keyword eg PROC
		sp::Token* newToken = new sp::Token(sp::Token::TokenType::NAME, tok->getLiteral());
		return new ast::VarName(newToken, tok->getLiteral());
	}
	throw this->genError("ParseVarName expected a NAME or Keyword, got: " + tok->getLiteral());
}

ast::ProcName* Parser::parseProcName() {
	sp::Token* tok = this->currToken;
	if (currTokenIsNameOrKeyword()) {
		// correct TokenType since original TokenType could be NAME or a keyword eg PROC
		sp::Token* newToken = new sp::Token(sp::Token::TokenType::NAME, tok->getLiteral());
		return new ast::ProcName(newToken, tok->getLiteral());
	}
	throw this->genError("ParseProcName expected a NAME or Keyword, got: " + tok->getLiteral());
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

bool Parser::currTokenIs(sp::Token::TokenType tok_type) {
	if (!this->currToken) {
		throw this->genError("currToken Error");
	}
	return this->currToken->getType() == tok_type;
}

bool Parser::currTokenIsNameOrKeyword() {
	if (!this->currToken) {
		throw this->genError("currTokenIsNameOrKeyword Error");
	}
	return this->currToken->getType() == sp::Token::TokenType::NAME || Parser::isKeyword(this->currToken);
}

bool Parser::peekTokenIs(sp::Token::TokenType tok_type) {
	if (!this->peekToken) {
		throw this->genError("peekToken Error");
	}
	return this->peekToken->getType() == tok_type;
}

bool Parser::peekTokenIsNameOrKeyword() {
	if (!this->peekToken) {
		throw this->genError("peekTokenIsNameOrKeyword Error");
	}
	return this->peekToken->getType() == sp::Token::TokenType::NAME || Parser::isKeyword(this->peekToken);
}

bool Parser::expectPeek(sp::Token::TokenType tok_type) {
	if (this->peekTokenIs(tok_type)) {
		this->nextToken();
		return true;
	}
	return false;
}

bool Parser::expectPeekIsNameOrKeyword() {
	if (this->peekTokenIsNameOrKeyword()) {
		this->nextToken();
		return true;
	}
	return false;
}

bool Parser::parseTest() {
	return false;
}

sp::Token* LexerStub::nextToken() {
	if (index >= this->tokens.size()) {
		int diff = index - this->tokens.size() + 1;
		return new sp::Token(sp::Token::TokenType::EOFF, "EOFF Count: " + std::to_string(diff)); //EOF
	}
	sp::Token* out = this->tokens[index];
	++index;
	return out;
}

bool Parser::isKeyword(sp::Token* tok) {
	std::unordered_set<sp::Token::TokenType> keywords {
		sp::Token::TokenType::PROC,
		sp::Token::TokenType::READ,
		sp::Token::TokenType::PRINT,
		sp::Token::TokenType::CALL,
		sp::Token::TokenType::WHILE,
		sp::Token::TokenType::IF,
		sp::Token::TokenType::THEN,
		sp::Token::TokenType::ELSE,
	};

	// if iterator returned from .find() == .end(), that means not found in set
	return keywords.find(tok->getType()) != keywords.end();
}
