#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator() {
}

vector<string> firstArgTypes = { VARIABLE_, NAME_, UNDERSCORE_ };
vector<string> secondArgTypes = { UNDERSCORE_, NAME_, EXPRESSION_, EXPRESSIONWITHUNDERSCORE_ };

bool PatternEvaluator::evaluate(unordered_map<string, string> declarations, Clause clause,
	unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    string varName = clause.getRel(); // i.e. varName(firstArg, secondArg)
    string firstType = getArgType(firstArg, declarations);
    string secondType = getArgType(secondArg, declarations);
    int queryType = getQueryType(firstType, secondType);
    vector<int> stmtNums = PKB::stmtTable->getAllAssignStmtNums();
    bool result;

    switch (queryType) {
    case nameUnderscore:
        result = evaluateNameUnderscore(stmtNums, varName, firstArg, tempResults);
        break;
    case nameName:
    case nameExpr:
        result = evaluateNameExpression(stmtNums, varName, firstArg, secondArg, tempResults);
        break;
    case nameExprWithUnderscore:
        result = evaluateNameExpressionWithUnderscore(stmtNums, varName, firstArg, secondArg, tempResults);
        break;
    case varUnderscore:
        result = evaluateVarUnderscore(stmtNums, varName, firstArg, tempResults);
        break;
    case variableName:
    case varExpr:
        result = evaluateVarExpression(stmtNums, varName, firstArg, secondArg, tempResults);
        break;
    case varExprWithUnderscore:
        result = evaluateVarExpressionWithUnderscore(stmtNums, varName, firstArg, secondArg, tempResults);
        break;
    case underscoreUnderscore:
        result = evaluateUnderscoreUnderscore(stmtNums, varName, tempResults);
        break;
    case underscoreName:
    case underscoreExpr:
        result = evaluateVarExpression(stmtNums, varName, firstArg, secondArg, tempResults);
        break;
    case underscoreExprWithUnderscore:
        result = evaluateUnderscoreExpressionWithUnderscore(stmtNums, varName, secondArg, tempResults);
        break;
    default:
        throw std::runtime_error("Arguments for pattern matching is invalid.");
        break;
    }
    return result;
}

// Function to print the index of an element
int PatternEvaluator::getIndex(vector<string> v, string s) {
    int result;
    vector<string>::iterator it = find(v.begin(), v.end(), s);
    if (it != v.end())
    {
        result = it - v.begin();
    }
    else {
        result = -1;
    }
    return result;
}

int PatternEvaluator::getQueryType(string firstType, string secondType) {
    int result;
    int firstDigit = getIndex(firstArgTypes, firstType);
    int secondDigit = getIndex(secondArgTypes, secondType);
    result = firstDigit * 10 + secondDigit;
    return result;
}

// function to type cast Stmt to AssignStmt
ast::AssignStmt* PatternEvaluator::typeCastAssignStmt(ast::Stmt* stmt) {
    ast::AssignStmt* pt = dynamic_cast<ast::AssignStmt*>(stmt);
    if (pt == nullptr) {
        throw std::runtime_error("Casting from stmt to assignStmt failed.");
    }
    return pt;
}

// function to type cast Expr to InfixExpr
ast::InfixExpr* PatternEvaluator::typeCastInfixExpr(ast::Expr* expr) {
    ast::InfixExpr* pt = dynamic_cast<ast::InfixExpr*>(expr);
    if (pt == nullptr) {
        throw std::runtime_error("Casting from expr to infixExpr failed.");
    }
    return pt;
}

// Compares two ASTs, returns true if AST1 == AST2
bool PatternEvaluator::areIdentical(ast::InfixExpr* root1, ast::InfixExpr* root2) {
    // base cases
    if (root1 == nullptr && root2 == nullptr)
        return true;

    if (root1 == nullptr || root2 == nullptr)
        return false;
    
    // Check if the data of both roots is same and data of left and right
    // subtrees are also same
    return (root1->getTokenLiteral() == root2->getTokenLiteral() &&
        exprAreIdentical(root1->getLeft(), root2->getLeft()) &&
        exprAreIdentical(root1->getRight(), root2->getRight()));
}

// Compares two ASTs, returns true if AST2 is a subexpression of AST1
bool PatternEvaluator::isSubtree(ast::InfixExpr* root1, ast::InfixExpr* root2) {
    if (root1 == nullptr || root2 == nullptr)
        return true;
    
    if (areIdentical(root1, root2))
        return true;
    
    // If the tree with root as current node doesn't match then try left
    // and right subtrees one by one
    return (exprIsSubtree(root1->getLeft(), root2) ||
        exprIsSubtree(root1->getRight(), root2));
}

bool PatternEvaluator::exprIsSubtree(ast::Expr* expr1, ast::Expr* expr2) {
    ast::InfixExpr* infixExpr1 = dynamic_cast<ast::InfixExpr*>(expr1);
    ast::ConstVal* constValExpr1 = dynamic_cast<ast::ConstVal*>(expr1);
    ast::VarName* varNameExpr1 = dynamic_cast<ast::VarName*>(expr1);
    ast::InfixExpr* infixExpr2 = dynamic_cast<ast::InfixExpr*>(expr2);
    ast::ConstVal* constValExpr2 = dynamic_cast<ast::ConstVal*>(expr2);
    ast::VarName* varNameExpr2 = dynamic_cast<ast::VarName*>(expr2);

    if (infixExpr1 != nullptr && infixExpr2 != nullptr) {
        return isSubtree(infixExpr1, infixExpr2);
    }
    else if (constValExpr1 != nullptr && constValExpr2 != nullptr) {
        return (constValExpr1->getVal() == constValExpr2->getVal());
    }
    else if (varNameExpr1 != nullptr && varNameExpr2 != nullptr) {
        return (varNameExpr1->getVal() == varNameExpr2->getVal());
    }
    return false;
}

bool PatternEvaluator::exprAreIdentical(ast::Expr* expr1, ast::Expr* expr2) {
    ast::InfixExpr* infixExpr1 = dynamic_cast<ast::InfixExpr*>(expr1);
    ast::ConstVal* constValExpr1 = dynamic_cast<ast::ConstVal*>(expr1);
    ast::VarName* varNameExpr1 = dynamic_cast<ast::VarName*>(expr1);
    ast::InfixExpr* infixExpr2 = dynamic_cast<ast::InfixExpr*>(expr2);
    ast::ConstVal* constValExpr2 = dynamic_cast<ast::ConstVal*>(expr2);
    ast::VarName* varNameExpr2 = dynamic_cast<ast::VarName*>(expr2);

    if (infixExpr1 != nullptr && infixExpr2 != nullptr) {
        return areIdentical(infixExpr1, infixExpr2);
    }
    else if (constValExpr1 != nullptr && constValExpr2 != nullptr) {
        return (constValExpr1->getVal() == constValExpr2->getVal());
    }
    else if (varNameExpr1 != nullptr && varNameExpr2 != nullptr) {
        return (varNameExpr1->getVal() == varNameExpr2->getVal());
    }
    return false;
}

// trim string with quotes
string trimQuotes(string s) {
    return trim(s.substr(1, s.size() - 2));
}

// trim string with quotes and underscore
string trimQuotesUnderscore(string s) {
    return trim(s.substr(2, s.size() - 4));
}

// Will get all assignment stmts that matches variable name on LHS
bool PatternEvaluator::evaluateNameUnderscore(vector<int> stmtNums, string varName, string firstArg,
    unordered_map<string, vector<int>>& tempResults) { // a("x", _)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        if (stmt->getName()->getVal() == trimQuotes(firstArg)) {
            tempResults[varName].push_back(*it);
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmts that fully matches LHS and RHS
bool PatternEvaluator::evaluateNameExpression(vector<int> stmtNums, string varName, string firstArg,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a("x", "y + z")
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = dynamic_cast<ast::AssignStmt*>(PKB::stmtTable->getStmtNode(*it));
        bool firstArgMatch = (stmt->getName()->getVal() == trimQuotes(firstArg));
        ast::Expr* expr1 = stmt->getExpr();

        /** begin ritual to Summon Parser **/
        string input = trimQuotes(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprAreIdentical(expr1, expr2);
        if (firstArgMatch && secondArgMatch) {
            tempResults[varName].push_back(*it);
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmts that fully matches LHS and RHS matches subexpr
bool PatternEvaluator::evaluateNameExpressionWithUnderscore(vector<int> stmtNums, string varName, string firstArg,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a("x", _"y + z"_)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        bool firstArgMatch = (stmt->getName()->getVal() == trimQuotes(firstArg));
        ast::Expr* expr1 = stmt->getExpr();

        /** begin ritual to Summon Parser **/
        string input = trimQuotesUnderscore(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprIsSubtree(expr1, expr2);
        if (firstArgMatch && secondArgMatch) {
            tempResults[varName].push_back(*it);
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS
bool PatternEvaluator::evaluateVarUnderscore(vector<int> stmtNums, string varName, string firstArg,
    unordered_map<string, vector<int>>& tempResults) { // a(v, _)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        tempResults[varName].push_back(*it); // store stmtNum
        string var = stmt->getName()->getVal();
        tempResults[firstArg].push_back(PKB::varTable->getVarID(var)); // store varID of var on LHS 
    }
    if (tempResults[varName].empty() || tempResults[firstArg].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS and checks if matches RHS
bool PatternEvaluator::evaluateVarExpression(vector<int> stmtNums, string varName, string firstArg,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a(v, "x + y")
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        ast::Expr* expr1 = stmt->getExpr();
        
        /** begin ritual to Summon Parser **/
        string input = trimQuotes(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprAreIdentical(expr1, expr2);
        if (secondArgMatch) {
            string var = stmt->getName()->getVal();
            tempResults[firstArg].push_back(PKB::varTable->getVarID(var)); // store varID of var on LHS
            tempResults[varName].push_back(*it); // store stmtNum
        }
    }
    if (tempResults[varName].empty() || tempResults[firstArg].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS and checks if its a subexpr of RHS
bool PatternEvaluator::evaluateVarExpressionWithUnderscore(vector<int> stmtNums, string varName, string firstArg,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a(v, _"x + y"_)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        ast::Expr* expr1 = stmt->getExpr();
        
        /** begin ritual to Summon Parser **/
        string input = trimQuotesUnderscore(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprIsSubtree(expr1, expr2);
        if (secondArgMatch) {
            string var = stmt->getName()->getVal();
            tempResults[firstArg].push_back(PKB::varTable->getVarID(var)); // store varID of var on LHS
            tempResults[varName].push_back(*it); // store stmtNum
        }
    }
    if (tempResults[varName].empty() || tempResults[firstArg].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmts
bool PatternEvaluator::evaluateUnderscoreUnderscore(vector<int> stmtNums, string varName,
    unordered_map<string, vector<int>>& tempResults) { // a(_, _)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        tempResults[varName].push_back(*it); // store all assignment stmts
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums that matches RHS
bool PatternEvaluator::evaluateUnderscoreExpression(vector<int> stmtNums, string varName,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a(_, "x + y")
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        ast::Expr* expr1 = stmt->getExpr();
        
        /** begin ritual to Summon Parser **/
        string input = trimQuotes(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprAreIdentical(expr1, expr2);
        if (secondArgMatch) {
            tempResults[varName].push_back(*it); // store stmtNum
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums that have subexpr that matches
bool PatternEvaluator::evaluateUnderscoreExpressionWithUnderscore(vector<int> stmtNums, string varName,
    string secondArg, unordered_map<string, vector<int>>& tempResults) {// a(_, _"x + y"_)
    vector<int>::iterator it = stmtNums.begin();
    for (it; it != stmtNums.end(); it++) {
        ast::AssignStmt* stmt = typeCastAssignStmt(PKB::stmtTable->getStmtNode(*it));
        ast::Expr* expr1 = stmt->getExpr();
        
        /** begin ritual to Summon Parser **/
        string input = trimQuotesUnderscore(secondArg);
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto lexerObj = new LexerStub(tok_ptrs);
        Parser parserObj = Parser(lexerObj);
        int precedence = ParserUtils::ExprPrecedence::LOWEST;
        ast::Expr* expr2 = parserObj.parseExpr(precedence);

        bool secondArgMatch = exprIsSubtree(expr1, expr2);
        if (secondArgMatch) {
            tempResults[varName].push_back(*it); // store stmtNum
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

PatternEvaluator::~PatternEvaluator() {

}
