#include <iostream>
#include <vector>

using namespace std;

#include "SP/Parser.h"
#include "SP/DesignExtractor.h"

Parser::Parser(LexerStub* l_ptr) : l_ptr{ l_ptr } {
    this->nextToken();
    this->nextToken();
}

void Parser::nextToken() {
    currToken = peekToken;
    peekToken = (*l_ptr).nextToken();
};

bool Parser::parse() {
    try {
        DesignExtractor::signalReset();
        ast::Program* prog = this->parseProgram();

        // check to ensure no calling undefined procedures
        ParserUtils::throwIfUndefCall(prog);

        // check to ensure no cyclical proc calls
        ParserUtils::throwIfCyclic(prog);

        // call DE
        vector<ast::Proc*> procs = prog->getProcedures();
        for (int i = 0; i < procs.size(); i++) {
            ast::Proc* procedure = procs[i];
            //store new procedure

            DesignExtractor::storeNewProcedure(procedure->getName()->getVal());
            vector<ast::Stmt*> stmts = procedure->getStmtLst()->getStatements();
            addStmtLstToDE(stmts, procedure);

            DesignExtractor::exitProcedure();
        }

        //end of populating DE
        DesignExtractor::signalEnd();


    } catch (sp::ParserException &ex) {
        //print error message
        cout << ex.what();
        //error encountered so reset PKB and DE
        DesignExtractor::signalReset();
        return false;
    } catch (sp::UtilsException &ex) {
        //print error message
        cout << ex.what();
        //error encountered so reset PKB and DE
        DesignExtractor::signalReset();
        return false;
    }

    return true;
}

void Parser::addStmtLstToDE(vector<ast::Stmt*> stmts, ast::Proc* proc) {
    for (StmtNum i = 0; i < stmts.size(); i++) {
        ast::Stmt* stmt = stmts[i];

        sp::Token::TokenType type = stmt->getToken()->getType();
        if (type == sp::Token::TokenType::ASSIGN) {
            ast::AssignStmt* ass = (ast::AssignStmt*)stmt;

            DesignExtractor::storeNewAssignment(ass->getIndex(), ass->getName()->getVal(), ass);
        } else if (type == sp::Token::TokenType::READ) {
            ast::ReadStmt* read = (ast::ReadStmt*)stmt;
            DesignExtractor::storeNewRead(read->getIndex(), read->getName()->getVal(), read);
        } else if (type == sp::Token::TokenType::PRINT) {
            ast::PrintStmt* print = (ast::PrintStmt*)stmt;

            DesignExtractor::storeNewPrint(print->getIndex(), print->getName()->getVal(), print);
        } else if (type == sp::Token::TokenType::WHILE) {
            ast::WhileStmt* whi = (ast::WhileStmt*)stmt;
            ast::CondExprBag* ce = (ast::CondExprBag*) whi->getCondExpr();
            DesignExtractor::storeNewWhile(whi->getIndex(), ce->getVarNames(), ce->getConstVal(), whi);
            vector<ast::Stmt*> whileStmts = whi->getStmtLst()->getStatements();
            addStmtLstToDE(whileStmts, proc);
            DesignExtractor::exitWhile();
        } else if (type == sp::Token::TokenType::IF) {

            ast::IfStmt* iff = (ast::IfStmt*)stmt;
            ast::CondExprBag* ce = (ast::CondExprBag*)iff->getCondExpr();
            DesignExtractor::storeNewIf(iff->getIndex(), ce->getVarNames(), ce->getConstVal(), iff);
            vector<ast::Stmt*> ifCon = iff->getConsequence()->getStatements();
            addStmtLstToDE(ifCon, proc);

            DesignExtractor::storeNewElse();
            vector<ast::Stmt*> ifAlt = iff->getAlternative()->getStatements();
            addStmtLstToDE(ifAlt, proc);
            DesignExtractor::endIfElse();

        } else if (type == sp::Token::TokenType::CALL) {
            //throw "NOT READY";
            ast::CallStmt* cs = (ast::CallStmt*)stmt;
            DesignExtractor::storeNewCall(cs->getIndex(), proc->getName()->getVal(),cs->getName()->getVal(), cs);
        } else {
            throw sp::ParseExprException("Parser call to DE", -1, "Unknown statement");
        }
    }
}



ast::Program* Parser::parseProgram() {
    std::vector<ast::Proc*> procs{};
    //to store proc names
    std::unordered_set<STRING> procnames{};
    while (this->currToken && (!this->currTokenIs(sp::Token::TokenType::EOFF))) {
        ast::Proc* proc = this->parseProc();
        if (!proc) {
            throw sp::ParserException(-1, "Error encountered when parsing Procedure");
        }

        //checking if proc exists
        if (procnames.find(proc->getName()->getVal()) != procnames.end()) {
            throw sp::ParserException(-1, "Procedure already exists: " + proc->getName()->getVal());
        } else {
            procnames.insert(proc->getName()->getVal());
        }

        procs.push_back(proc);

    }
    if (procs.size() == 0) {
        throw sp::ParserException(-1, "Expect at least one procedure");
    }

    return new ast::Program(procs[0], procs);

}

ast::Proc* Parser::parseProc() {
    if (!this->currTokenIs(sp::Token::TokenType::PROC)) {
        throw sp::ParserException(-1, "ParseProc expected a PROC, got: " + this->currLiteral());
    }
    sp::Token* proc_tok = this->currToken;

    if (!this->expectPeekIsNameOrKeyword()) {
        throw sp::ParserException(-1, "ParseProc expected a NAME or Keyword, got: " + this->peekLiteral());
    }
    ast::ProcName* pn = this->parseProcName();

    if (!this->expectPeek(sp::Token::TokenType::LBRACE)) {
        throw sp::ParserException(-1, "ParseProc expected a LBRACE, got: " + this->peekLiteral());
    }

    //current token is {
    this->nextToken();

    ast::StmtLst* stmtlst = this->parseStmtLst();

    if (stmtlst->getStatements().size() == 0) {
        throw sp::ParserException(-1, "Procedure " + pn->getVal() + " should have at least one statement.");
    }

    if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
        throw sp::ParserException(-1, "ParseProc expected a RBRACE, got: " + this->peekLiteral());
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
    if (this->currTokenIsNameOrKeyword()) {
        return parseVarName();
    } else if (tok->getType() == sp::Token::TokenType::CONST) {
        return parseConstVal();
    } else if (tok->getType() == sp::Token::TokenType::LPAREN) {
        return parseLParenPrefixExpr();
    }
    throw this->genExprError("ParsePrefixExpr expected a Prefix, NAME or CONST got: " + tok->getLiteral());
}

ast::Expr* Parser::parseInfixExpr(ast::Expr* left_expr) {
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

ast::CondExpr* Parser::parseCondExpr(int precedence) {
    ast::CondExprBag* ceb = new ast::CondExprBag(new sp::Token(sp::Token::TokenType::BOOL, "COND_EXPR_BAG"));

    // expect first part to always be (
    // since if ( ... )
    if (!this->currTokenIs(sp::Token::TokenType::LPAREN)) {
        throw this->genCondExprError("ParseCondExpr expected LPAREN instead encountered: " + this->currLiteral());
    }
    ceb->pushToken(this->currToken);
    this->nextToken();

    int l_parens = 1;
    while (l_parens > 0) {
        if (this->currTokenIs(sp::Token::TokenType::EOFF)) { break; }
        auto tok = this->currToken;
        if (this->currTokenIs(sp::Token::TokenType::LPAREN)) { l_parens++; }
        if (this->currTokenIs(sp::Token::TokenType::RPAREN)) { l_parens--; }
        if (l_parens <= 0) { break; }	// dont add to bag
        ceb->pushToken(tok);
        // nextToken
        this->nextToken();
    }
    if (!this->currTokenIs(sp::Token::TokenType::RPAREN)) {
        throw this->genError("ParseCondExpr expected RPAREN instead encountered: " + this->currLiteral());
    }
    ceb->pushToken(this->currToken);
    std::vector<sp::Token*> tokens;
    CondExprUtils::VectorShallowCopy(ceb->getTokens(), tokens);

    // check if legal or throw exception
    try {
        CondExprUtils::ParseCondExpr(tokens);
    }
    catch (sp::UtilsException& ex) {
        throw this->genCondExprError(ex.what());
    }
    return ceb;
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

    if (stmt_lst->getStatements().size() == 0) {
        throw this->genError("While should have at least one statement.");
    }

    if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
        throw this->genError("ParseWhile expected RBRACE }, encountered: " + this->currLiteral());
    }

    return new ast::WhileStmt(curr_index, tok, cond_expr, stmt_lst);
}

ast::IfStmt* Parser::parseIfStmt() {
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

    if (csq_lst->getStatements().size() == 0) {
        throw this->genError("If should have at least one statement.");
    }

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

    if (alt_lst->getStatements().size() == 0) {
        throw this->genError("Else should have at least one statement.");
    }

    if (!this->currTokenIs(sp::Token::TokenType::RBRACE)) {
        throw this->genError("ParseWhile expected RBRACE }, encountered: " + this->currLiteral());
    }

    return new ast::IfStmt(curr_index, tok, cond_expr, csq_lst, alt_lst);
}

sp::ParserException Parser::genError(STRING str) {
    return sp::ParserException(this->pc, str);
}

sp::ParserException Parser::genExprError(STRING str) {
    return sp::ParseExprException(this->pc, str);
}

sp::ParserException Parser::genCondExprError(STRING str) {
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

STRING Parser::peekLiteral() {
    if (!this->peekToken) {
        return "nullptr:??";
    }
    return this->peekToken->getLiteral();
}

STRING Parser::currLiteral() {
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
