#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
#include "SP/Parser.h"

namespace TokenUtils {

}

Parser::Parser(LexerStub* l_ptr) : l_ptr{l_ptr} {
	//this->registerInfixExpr(sp::Token::TokenType::NAME, &Parser::parseVarNameExpr);
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

	if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
		throw this->genError("ParseProc expected a RBRACE, got: " + this->peekLiteral()); 
	}
	//current token is }
	this->nextToken();

	return new ast::Proc(proc_tok, pn, stmtlst);
}

ast::StmtLst* Parser::parseStmtLst() {
	std::vector<ast::Stmt*> xs{};
	while (this->currToken && (!this->currTokenIs(sp::Token::TokenType::EOFF))) {
		if (this->currTokenIs(sp::Token::TokenType::RBRACE)) {
			// if encouter } means end of stmtLst
			break;
		}

		ast::Stmt* stmt = this->parseStmt();
		if (!stmt) { throw this->genError("ParseStmtLst error"); }
		xs.push_back(stmt);

		// currToken is semicolon or RBRACE (if WHILE or IF which dont end with ;)
		if (!this->currTokenIs(sp::Token::TokenType::SEMICOLON) && !this->currTokenIs(sp::Token::TokenType::RBRACE)) { 
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
	} else if (this->currTokenIsNameOrKeyword() && this->peekTokenIs(sp::Token::TokenType::ASSIGN)) {
		// this clause must be close to the top, precedence over the other keyword parse methods
		return this->parseAssignStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::CALL)) {
		return this->parseCallStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::PRINT)) {
		return this->parsePrintStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::READ)) {
		return this->parseReadStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::WHILE)) {
		return this->parseWhileStmt();
	} else if (this->currTokenIs(sp::Token::TokenType::IF)) {
		return this->parseIfStmt();
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

	// placeholder block for without parseExpr
	//while (!this->currTokenIs(sp::Token::TokenType::SEMICOLON)) {
	//	this->nextToken();
	//}

	/****				The following is for parseExpr						****/
	this->nextToken();	// curr Token is =
	ast::Expr* ass_expr = this->parseExpr(ParserUtils::ExprPrecedence::LOWEST);
	this->nextToken();	// curr Token is last Expr on RHS, next to get semicolon as curr
	/****				End parseExpr										****/


	// when returning, currToken must be semicolon
	if (!this->currTokenIs(sp::Token::TokenType::SEMICOLON)) { 
		throw this->genError("ParseAssign expected Semicolon, got: " + this->currLiteral());
	}
	// this is here instead of lower to ensure StmtNum is more accurate

	return new ast::AssignStmt(this->getPlusPC(), assToken, vn, ass_expr); // expr not calc yet
	//// below to check for AssignStmt printouts if needed
	//ast::AssignStmt* ass = new ast::AssignStmt(this->getPlusPC(), assToken, vn, ass_expr);
	//std::cout << "StmtNum: " + std::to_string(ass->getIndex()) + " - " + ass->toString() << std::endl;
	//return ass;
}


ast::Expr* Parser::parseExpr(int precedence) {
	// e.g. y = x + 1; this deals with the x
	auto left_expr = parsePrefixExpr(this->currToken);

	// if y = x; will encounter semicolon and just return
	// the higher the precedence the deeper it is in the tree
	while (!this->peekTokenIs(sp::Token::TokenType::SEMICOLON) && precedence < this->peekPrecedence()) {
		this->nextToken();
		left_expr = this->parseInfixExpr(left_expr);
	}

	// we may encounter an invalid symbol like RPAREN here or SEMICOLON and thats ok, just return
	// let the caller deal with it, LPAREN parsing needs this
	return left_expr;
}

// basically a switch for choosing parsing VarName or ConstVal
ast::Expr* Parser::parsePrefixExpr(sp::Token* tok) {
	//throw "NOT READY";
	//if (tok->getType() == sp::Token::TokenType::NAME) {
	if (this->currTokenIsNameOrKeyword()) {
		return parseVarName();
	}
	else if (tok->getType() == sp::Token::TokenType::CONST) {
		return parseConstVal();
	}
	else if (tok->getType() == sp::Token::TokenType::LPAREN) {
		return parseLParenPrefixExpr();
	}
	throw this->genExprError("ParsePrefixExpr expected a Prefix, NAME or CONST got: " + tok->getLiteral());
}

ast::Expr* Parser::parseInfixExpr(ast::Expr* left_expr) {
	//throw "NOT READY";
	auto tok = this->currToken;
	if (!ParserUtils::hasExprRank(tok->getType())) {
		throw this->genExprError("ParseInfixExpr expected a Infix Operator, got: " + tok->getLiteral());
	}
	// expr is a valid infix expr

	// need to rmb whch operator comes first, curr one, or next
	auto curr_precedence = this->currPrecedence();
	this->nextToken();
	auto right_expr = this->parseExpr(curr_precedence);
	return new ast::InfixExpr(tok, left_expr, right_expr);

}

ast::Expr* Parser::parseLParenPrefixExpr() {
	sp::Token* tok = this->currToken;
	if (tok->getType() != sp::Token::TokenType::LPAREN) {
		throw this->genError("ParseLParen expected LPAREN instead encountered: " + tok->getLiteral());
	}
	this->nextToken();
	ast::Expr* expr = this->parseExpr(ParserUtils::ExprPrecedence::LOWEST);
	this->nextToken();	// shift away the last expr within the RParen, curr should now be RPAREN

	// the currToken should be RPAREN, since we just shifted away the last expr in RParen
	if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
		throw this->genExprError("ParseLParen expected RPAREN instead encountered: " + this->currLiteral());
	}
	return expr;
}

// honestly no idea if this is even needed
// for test case to catch `a == b) && (c < 5)` output is `a == b` but wont other parts of parser catch?
ast::CondExpr* Parser::parseCondExpr(int precedence) {
	return this->parseCondExprInner(precedence);
	//auto cond_expr = this->parseCondExprInner(precedence);
	////std::cout << this->currLiteral() << std::endl;
	////std::cout << this->peekToken->getLiteral() << std::endl;
	//this->nextToken(); // currToken is )
	//auto peek_tok = this->peekToken;
	//if (ParserUtils::isRelOps(peek_tok->getType()) || ParserUtils::isCondExprOps(peek_tok->getType())) {
	//	throw this->genCondExprError("ParseCondExpr encountered unexpected: " + peek_tok->getLiteral());
	//}
	//return cond_expr;
}

ast::CondExpr* Parser::parseCondExprInner(int precedence) {
	//throw "not ready, everything below this is unchanged";
	// e.g. (1) == a
	bool isStartWithParen = false;
	auto start_tok = this->currToken;
	if (this->currTokenIs(sp::Token::TokenType::LPAREN)) {
		isStartWithParen = true;
	}
	auto left_expr = parsePrefixCondExpr();

	// e.g. a + 1 > c % 2
	if (ParserUtils::hasExprRank(this->peekToken->getType())) {
		this->nextToken(); // shift + to current
		ast::Expr* left_unwrap;
		try {
			left_unwrap = ((ast::CondExprWrapper*)left_expr)->unWrap();
		}
		catch (std::exception ex) {
			throw this->genCondExprError("parseCondExprInner expected CondExprWrapper 0 instead got: " + left_expr->getTokenLiteral());
		}
		if (!left_unwrap) {
			throw this->genCondExprError("parseCondExprInner expected CondExprWrapper 4 instead got: " + left_expr->getTokenLiteral());
		}
		left_expr = ast::CondExprWrapper::wrap(this->parseInfixExpr(left_unwrap));
	}

	if (ParserUtils::isRelOps(this->peekToken->getType())) {
		this->nextToken(); // shift relToken to current
		ast::Expr* left_unwrap;
		try {
			left_unwrap = ((ast::CondExprWrapper*)left_expr)->unWrap();
		}
		catch (std::exception ex) {
			throw this->genCondExprError("parseCondExprInner expected CondExprWrapper instead got: " + left_expr->getTokenLiteral());
		}
		if (!left_expr) {
			throw this->genCondExprError("parseCondExprInner expected CondExprWrapper 2 instead got: " + left_expr->getTokenLiteral());
		}
		left_expr = this->parseRelExpr(left_unwrap);
	}

	//std::cout << this->currLiteral() << std::endl;
	// must be (a > b) && (c == d) the first ) before the &&, first LPAREN ??
	if (!this->currTokenIs(sp::Token::TokenType::RPAREN) && ParserUtils::isCondExprOps(this->peekToken->getType())) {
		throw this->genCondExprError("parseCondExprInner expected ) before && instead got: " + this->currLiteral());
	}
	// for scenario !(a + 1 > c % 2) || (a != b), need wrap ( ... ) around !
	if (!isStartWithParen && ParserUtils::isCondExprOps(this->peekToken->getType())) {
		throw this->genCondExprError("parseCondExprInner expected ( ... ) before && instead got: " + start_tok->getLiteral());
	}

	// if y = x; will encounter semicolon and just return
	// the higher the precedence the deeper it is in the tree
	//while (!this->peekTokenIs(sp::Token::TokenType::SEMICOLON) && precedence < this->peekPrecedence()) {
	while (ParserUtils::isCondExprOps(this->peekToken->getType())) {
		this->nextToken();	// && now at curr
		left_expr = this->parseInfixCondExpr(left_expr);
	}

	// we may encounter an invalid symbol like RPAREN here or SEMICOLON and thats ok, just return
	// let the caller deal with it, LPAREN parsing needs this
	return left_expr;
}

ast::CondExpr* Parser::parseInfixCondExpr(ast::CondExpr* left_expr) {
	//throw "not ready, everything below this is unchanged";
	auto tok = this->currToken;
	if (!ParserUtils::isCondExprOps(tok->getType())) {
		throw this->genCondExprError("ParseInfixCondExpr expected && or || , got: " + tok->getLiteral());
	}
	// expr is a valid infix expr

	// need to rmb whch operator comes first, curr one, or next
	auto curr_precedence = ParserUtils::CondExprPrecedence::LOWEST;//this->currPrecedence();
	this->nextToken();

	// should be the 2nd ( here or throw error, (a == b) && (c > 0)
	if (!this->currTokenIs(sp::Token::TokenType::LPAREN)) {
		throw this->genCondExprError("ParseInfixCondExpr expected LPAREN ( , got: " + this->currLiteral());
	}

	auto right_expr = this->parseCondExprInner(curr_precedence);
	return new ast::InfixCondExpr(tok, left_expr, right_expr);
}

// basically a switch for choosing parsing VarName or ConstVal
ast::CondExpr* Parser::parsePrefixCondExpr() {
	//throw "not ready, everything below this is unchanged";
	auto tok = this->currToken;
	if (this->currTokenIsNameOrKeyword()) {
		return ast::CondExprWrapper::wrap(parseVarName());
		// go to LParenExpr, ?? peek for which and dispatch? or isit parseCondExprInner that does this
	}
	else if (tok->getType() == sp::Token::TokenType::CONST) {
		return ast::CondExprWrapper::wrap(parseConstVal());
	}
	else if (tok->getType() == sp::Token::TokenType::LPAREN) {
		return parseLParenPrefixCondExpr();
	}
	else if (tok->getType() == sp::Token::TokenType::NOT) {
		return parseNotExpr();
	}
	throw this->genExprError("ParsePrefixExpr expected a Prefix, NAME or CONST got: " + tok->getLiteral());
}

ast::CondExpr* Parser::parseRelExpr(ast::Expr* left_expr) {
	//throw "not ready, everything below this is unchanged";

	auto tok = this->currToken;
	if (!ParserUtils::isRelOps(tok->getType())) {
		throw this->genCondExprError("ParseRelExpr expected && or ||, got: " + tok->getLiteral());
	}
	// expr is a valid Rel Expr

	// need to rmb whch operator comes first, curr one, or next
	auto curr_precedence = ParserUtils::ExprPrecedence::LOWEST;//this->currPrecedence();
	this->nextToken();
	// or should it be LParenPrefix?
	auto right_expr = this->parseExpr(curr_precedence);
	return new ast::RelExpr(tok, left_expr, right_expr);
}

ast::CondExpr* Parser::parseNotExpr() {
	//throw "not ready, everything below this is unchanged";
	sp::Token* tok = this->currToken;
	if (tok->getType() != sp::Token::TokenType::NOT) {
		throw this->genCondExprError("ParseNotExpr expected NOT instead encountered: " + tok->getLiteral());
	}
	// here currToken is !
	this->nextToken();
	// here currToken should be (

	// curr shouldve be (
	if (!this->currTokenIs(sp::Token::TokenType::LPAREN)) {
		throw this->genCondExprError("ParseNotExpr expected LPAREN ( instead encountered: " + this->currLiteral());
	}
	//ast::CondExpr* expr = this->parseCondExprInner(ParserUtils::CondExprPrecedence::LOWEST);
	ast::CondExpr* expr = this->parseLParenPrefixCondExpr();
	if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
		throw this->genCondExprError("ParseNotExpr expected RPAREN ) instead encountered: " + this->currLiteral());
	}
	return new ast::PrefixCondExpr(tok, expr);
}

// pretty much a clone of parseLParenPrefixExpr
ast::CondExpr* Parser::parseLParenPrefixCondExpr() {
	//throw "not ready, everything below this is unchanged";
	sp::Token* tok = this->currToken;
	if (tok->getType() != sp::Token::TokenType::LPAREN) {
		throw this->genCondExprError("ParseLParenCond expected LPAREN instead encountered: " + tok->getLiteral());
	}
	this->nextToken();
	ast::CondExpr* expr = this->parseCondExprInner(ParserUtils::CondExprPrecedence::LOWEST);
	this->nextToken();	// shift away the last expr within the RParen, curr should now be RPAREN

	// the currToken should be RPAREN, since we just shifted away the last expr in RParen
	if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
		throw this->genCondExprError("ParseLParenCond expected RPAREN instead encountered: " + this->currLiteral());
	}
	return expr;
}

ast::WhileStmt* Parser::parseWhileStmt() {
	int curr_index = this->getPlusPC();
	auto tok = this->currToken;
	if (!this->currTokenIs(sp::Token::TokenType::WHILE)) {
		throw this->genError("ParseWhile expected WHILE, encountered: " + this->currLiteral());
	}
	if (!this->expectPeek(sp::Token::TokenType::LPAREN)) {
		throw this->genError("ParseWhile expected LPAREN (, encountered: " + this->peekToken->getLiteral());
	}
	auto cond_expr = this->parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);

	if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
		throw this->genError("ParseWhile expected RPAREN ), encountered: " + this->currLiteral());
	}

	if (!this->expectPeek(sp::Token::TokenType::LBRACE)) {
		throw this->genError("ParseWhile expected LBRACE {, encountered: " + this->peekToken->getLiteral());
	}
	this->nextToken();

	auto stmt_lst = this->parseStmtLst();

	if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
		throw this->genError("ParseWhile expected RBRACE }, encountered: " + this->currLiteral());
	}

	return new ast::WhileStmt(curr_index, tok, cond_expr, stmt_lst);
}

ast::IfStmt* Parser::parseIfStmt() {
	//throw "NOT READY";
	auto tok = this->currToken;
	int curr_index = this->getPlusPC();
	if (!this->currTokenIs(sp::Token::TokenType::IF)) {
		throw this->genError("ParseIf expected IF, encountered: " + this->currLiteral());
	}
	if (!this->expectPeek(sp::Token::TokenType::LPAREN)) {
		throw this->genError("ParseIf expected LPAREN (, encountered: " + this->peekToken->getLiteral());
	}
	auto cond_expr = this->parseCondExpr(ParserUtils::CondExprPrecedence::LOWEST);

	if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
		throw this->genError("ParseIf expected RPAREN ), encountered: " + this->currLiteral());
	}

	if (!this->expectPeek(sp::Token::TokenType::THEN)) {
		throw this->genError("ParseIf expected THEN, encountered: " + this->peekToken->getLiteral());
	}

	if (!this->expectPeek(sp::Token::TokenType::LBRACE)) {
		throw this->genError("ParseIf expected LBRACE {, encountered: " + this->peekToken->getLiteral());
	}
	this->nextToken();

	auto csq_lst = this->parseStmtLst();

	if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
		throw this->genError("ParseIf expected RBRACE }, encountered: " + this->currLiteral());
	}

	if (!this->expectPeek(sp::Token::TokenType::ELSE)) {
		throw this->genError("ParseIf expected ELSE, encountered: " + this->peekToken->getLiteral());
	}

	if (!this->expectPeek(sp::Token::TokenType::LBRACE)) {
		throw this->genError("ParseWhile expected LBRACE {, encountered: " + this->peekToken->getLiteral());
	}
	this->nextToken();

	auto alt_lst = this->parseStmtLst();

	if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
		throw this->genError("ParseWhile expected RBRACE }, encountered: " + this->currLiteral());
	}

	return new ast::IfStmt(curr_index, tok, cond_expr, csq_lst, alt_lst);
}

//std::string Parser::genError(std::string str) {
sp::ParserException Parser::genError(std::string str) {
	//return "StmtNum: " + std::to_string(this->pc) + " - " + str;
	return sp::ParserException(this->pc, str);
}

sp::ParserException Parser::genExprError(std::string str) {
	return sp::ParseExprException(this->pc, str);
}

sp::ParserException Parser::genCondExprError(std::string str) {
	return sp::ParseCondExprException(this->pc, str);
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

ast::ConstVal* Parser::parseConstVal() {
	sp::Token* tok = this->currToken;
	if (currTokenIs(sp::Token::TokenType::CONST)) {
		int tmp;
		try {
			tmp = std::stoi(tok->getLiteral());
		}
		catch (std::exception& e) {
			// exception trying to convert to string
			throw "ParseConstVal: unable to convert token literal to strint, got: " + tok->getLiteral();
		}
		return new ast::ConstVal(tok, tmp);
	}
	throw this->genError("ParseConstVal expected a CONST, got: " + tok->getLiteral());
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

int Parser::peekPrecedence() {
	if (ParserUtils::hasExprRank(this->peekToken->getType())) {
		return ParserUtils::getExprRankUnsafe(this->peekToken->getType());
	}
	// no rank found, everything else takes precedence
	return ParserUtils::ExprPrecedence::LOWEST;
}

int Parser::currPrecedence() {
	if (ParserUtils::hasExprRank(this->currToken->getType())) {
		return ParserUtils::getExprRankUnsafe(this->currToken->getType());
	}
	return ParserUtils::ExprPrecedence::LOWEST;
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
	return ParserUtils::isKeyword(tok->getType());
}
