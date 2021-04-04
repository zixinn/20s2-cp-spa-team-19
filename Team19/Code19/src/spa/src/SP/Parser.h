#pragma once

#include <unordered_map>
#include <unordered_set>
#include "SP/Token.h"
#include "AST/Index.h"
#include "SP/ParserUtils.h"
#include "SP/CondExprUtils.h"
#include "SP/ParserException.h"

using namespace std;

class LexerStub {
    const std::vector<sp::Token*> tokens;
    int index = 0;
public:
    LexerStub(std::vector<sp::Token*> tokens) :tokens{ tokens } {};
    sp::Token* nextToken();
};

// Parses SIMPLE source code into an AST
// This includes validating the SIMPLE source, checking for recursive/cyclic procedural calls
class Parser {
    LexerStub* l_ptr;
    sp::Token* currToken = nullptr; // "curr_peek"
    sp::Token* peekToken = nullptr; //"init_peek";
    int pc = 1;
public:
    Parser(LexerStub* l_ptr);
    void nextToken();
    // Functions to parse statements and program design entities
    ast::VarName* parseVarName();
    ast::ProcName* parseProcName();
    ast::ConstVal* parseConstVal();
    ast::StmtLst* parseStmtLst();
    ast::Stmt* parseStmt();
    ast::AssignStmt* parseAssignStmt();
    ast::CallStmt* parseCallStmt();
    ast::ReadStmt* parseReadStmt();
    ast::PrintStmt* parsePrintStmt();
    ast::Proc* parseProc();
    ast::Program* parseProgram();
    ast::Expr* parseExpr(int precedence);
    ast::CondExpr* parseCondExpr(int precedence);
    ast::WhileStmt* parseWhileStmt();
    ast::IfStmt* parseIfStmt();

    //parse the whole thing including calls to DE
    //Return true if no error encountered, else false
    bool parse();

    inline sp::Token* getCurrToken() { return currToken; };
    static bool isKeyword(sp::Token* tok);

private:
    // Utility functions for Parsing
    bool currTokenIs(sp::Token::TokenType tok_type);
    bool expectPeek(sp::Token::TokenType tok_type);
    bool peekTokenIs(sp::Token::TokenType tok_type);

    bool currTokenIsNameOrKeyword();
    bool expectPeekIsNameOrKeyword();
    bool peekTokenIsNameOrKeyword();

    int getPlusPC();

    // Generates custom Parser exceptions
    sp::ParserException genError(STRING str);
    sp::ParserException genExprError(STRING str);
    sp::ParserException genCondExprError(STRING str);

    STRING currLiteral();
    STRING peekLiteral();
    bool parseTest();

    // Parses various expressions
    ast::Expr* parsePrefixExpr(sp::Token* tok);
    ast::Expr* parseInfixExpr(ast::Expr*);
    ast::Expr* parseLParenPrefixExpr();

    int peekPrecedence();
    int currPrecedence();

    // calls to DE
    void addStmtLstToDE(vector<ast::Stmt*> stmts, ast::Proc* proc);
};
