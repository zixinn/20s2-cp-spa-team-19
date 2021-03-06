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
        result = evaluateUnderscoreExpression(stmtNums, varName, secondArg, tempResults);
        break;
    case underscoreExprWithUnderscore:
        result = evaluateUnderscoreExpressionWithUnderscore(stmtNums, varName, secondArg, tempResults);
        break;
    default:
        throw std::runtime_error("Arguments for pattern matching is invalid.");
    }
    return result;
}

// Function to print the index of an element
int PatternEvaluator::getIndex(vector<string> v, string s) {
    int result;
    vector<string>::iterator it = find(v.begin(), v.end(), s);
    if (it != v.end()) {
        result = it - v.begin();
    } else {
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

string PatternEvaluator::parseExprToExprStr(string input) {
    /** begin ritual to Summon Parser **/
    std::vector<sp::Token> actual_tok;
    std::vector<sp::Token*> tok_ptrs;
    ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
    auto lexerObj = new LexerStub(tok_ptrs);
    Parser parserObj = Parser(lexerObj);
    int precedence = ParserUtils::ExprPrecedence::LOWEST;
    ast::Expr* expr = parserObj.parseExpr(precedence);
    return expr->toString();
}

// trim string with quotes
string trimQuotes(string s) {
    return trim(s.substr(1, s.size() - 2));
}

// trim string with quotes and underscore
string trimQuotesUnderscore(string s) {
    string underscoreRemoved = trim(s.substr(1, s.size() - 2));
    return trim(underscoreRemoved.substr(1, underscoreRemoved.size() - 2));
}

// Will get all assignment stmts that matches variable name on LHS
bool PatternEvaluator::evaluateNameUnderscore(vector<int> stmtNums, string varName, string firstArg,
    unordered_map<string, vector<int>>& tempResults) { // a("x", _)
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        if (varStr == trimQuotes(firstArg)) {
            tempResults[varName].push_back(stmtNum);
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
    string exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        string exprStr1 = p.second;

        if (varStr == trimQuotes(firstArg) && exprStr1 == exprStr2) {
            tempResults[varName].push_back(stmtNum);
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
    string exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        string exprStr1 = p.second;

        if (varStr == trimQuotes(firstArg) && exprStr1.find(exprStr2) != string::npos) {
            tempResults[varName].push_back(stmtNum);
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
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        tempResults[varName].push_back(stmtNum); // store stmtNum
        tempResults[firstArg].push_back(PKB::varTable->getVarID(varStr)); // store varID of var on LHS
    }
    if (tempResults[varName].empty() || tempResults[firstArg].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS and checks if matches RHS
bool PatternEvaluator::evaluateVarExpression(vector<int> stmtNums, string varName, string firstArg,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a(v, "x + y")
    string exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        string exprStr1 = p.second;

        if (exprStr1 == exprStr2) {
            tempResults[firstArg].push_back(PKB::varTable->getVarID(varStr)); // store varID of var on LHS
            tempResults[varName].push_back(stmtNum); // store stmtNum
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
    string exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string varStr = p.first;
        string exprStr1 = p.second;

        if (exprStr1.find(exprStr2) != string::npos) {
            tempResults[firstArg].push_back(PKB::varTable->getVarID(varStr)); // store varID of var on LHS
            tempResults[varName].push_back(stmtNum); // store stmtNum
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
    for (int& stmtNum : stmtNums) {
        tempResults[varName].push_back(stmtNum); // store all assignment stmts
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums that matches RHS
bool PatternEvaluator::evaluateUnderscoreExpression(vector<int> stmtNums, string varName,
    string secondArg, unordered_map<string, vector<int>>& tempResults) { // a(_, "x + y")
    string exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string exprStr1 = p.second;

        if (exprStr1 == exprStr2) {
            tempResults[varName].push_back(stmtNum); // store stmtNum
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
    string exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (int& stmtNum : stmtNums) {
        pair<string, string> p = PKB::stmtTable->getAssignExpr(stmtNum);
        string exprStr1 = p.second;

        if (exprStr1.find(exprStr2) != string::npos) {
            tempResults[varName].push_back(stmtNum); // store stmtNum
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

PatternEvaluator::~PatternEvaluator() {

}
