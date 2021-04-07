#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator() {
}

vector<STRING> firstArgTypes = { VARIABLE_, NAME_, UNDERSCORE_ };
vector<STRING> secondArgTypes = { UNDERSCORE_, NAME_, EXPRESSION_, EXPRESSIONWITHUNDERSCORE_ };

bool PatternEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                                unordered_map<STRING, vector<int>>& tempResults) {
    bool result;
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING varName = clause.getRel(); // i.e. varName(firstArg, secondArg)
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);
    STRING varNameType = getArgType(varName, declarations);

    if (varNameType == IF_) {
        result = evaluateIfClause(varName, firstArg, firstType, tempResults);
    } else if (varNameType == WHILE_) {
        result = evaluateWhileClause(varName, firstArg, firstType, tempResults);
    } else {
        int queryType = getQueryType(firstType, secondType);
        vector<StmtNum> stmtNums = PKB::stmtTable->getAllAssignStmtNums();

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
    }
    return result;
}

// Function to print the index of an element
int PatternEvaluator::getIndex(vector<STRING> v, STRING s) {
    int result;
    vector<STRING>::iterator it = find(v.begin(), v.end(), s);
    if (it != v.end()) {
        result = it - v.begin();
    } else {
        result = -1;
    }
    return result;
}

int PatternEvaluator::getQueryType(STRING firstType, STRING secondType) {
    int result;
    int firstDigit = getIndex(firstArgTypes, firstType);
    int secondDigit = getIndex(secondArgTypes, secondType);
    result = firstDigit * 10 + secondDigit;
    return result;
}

STRING PatternEvaluator::parseExprToExprStr(STRING input) {
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

// trim STRING with quotes
STRING trimQuotes(STRING s) {
    return trim(s.substr(1, s.size() - 2));
}

// trim STRING with quotes and underscore
STRING trimQuotesUnderscore(STRING s) {
    STRING underscoreRemoved = trim(s.substr(1, s.size() - 2));
    return trim(underscoreRemoved.substr(1, underscoreRemoved.size() - 2));
}

bool PatternEvaluator::evaluateIfClause(STRING varName, STRING firstArg, STRING firstType,
                                        unordered_map<STRING, vector<int>>& tempResults) {
    if (firstType != NAME_) { // variable/underscore
        pair<vector<StmtNum>, vector<VarID>> allIfPatterns = PKB::stmtTable->getAllIfPatterns();
        if (allIfPatterns.first.empty()) {
            return false;
        }
        tempResults[varName] = allIfPatterns.first;
        if (firstType != UNDERSCORE_) {
            tempResults[firstArg] = allIfPatterns.second;
        }
    } else { // first arg is name in quotation marks
        STRING varSTRING = trimQuotes(firstArg);
        VarID id = PKB::varTable->getVarID(varSTRING);
        if (id == -1) {
            return false;
        }
        unordered_set<VarID> vars = PKB::stmtTable->getIfStmtsWithControlVar(id);
        if (vars.empty()) {
            return false;
        }
        tempResults[varName].assign(vars.begin(), vars.end());
    }
    return true;
}

bool PatternEvaluator::evaluateWhileClause(STRING varName, STRING firstArg, STRING firstType,
                                           unordered_map<STRING, vector<int>>& tempResults) {
    if (firstType != NAME_) { // variable/underscore
        pair<vector<StmtNum>, vector<VarID>> allWhilePatterns = PKB::stmtTable->getAllWhilePatterns();
        if (allWhilePatterns.first.empty()) {
            return false;
        }
        tempResults[varName] = allWhilePatterns.first;
        if (firstType != UNDERSCORE_) {
            tempResults[firstArg] = allWhilePatterns.second;
        }
    } else { // first arg is name in quotation marks
        STRING varSTRING = trimQuotes(firstArg);
        VarID id = PKB::varTable->getVarID(varSTRING);
        if (id == -1) {
            return false;
        }
        unordered_set<VarID> vars = PKB::stmtTable->getWhileStmtsWithControlVar(id);
        if (vars.empty()) {
            return false;
        }
        tempResults[varName].assign(vars.begin(), vars.end());
    }
    return true;
}

// Will get all assignment stmts that matches variable name on LHS
bool PatternEvaluator::evaluateNameUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                              unordered_map<STRING, vector<int>>& tempResults) { // a("x", _)
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
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
bool PatternEvaluator::evaluateNameExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                              STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) { // a("x", "y + z")
    STRING exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
        STRING exprStr1 = p.second;

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
bool PatternEvaluator::evaluateNameExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                            STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) { // a("x", _"y + z"_)
    STRING exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
        STRING exprStr1 = p.second;

        if (varStr == trimQuotes(firstArg) && exprStr1.find(exprStr2) != STRING::npos) {
            tempResults[varName].push_back(stmtNum);
        }
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS
bool PatternEvaluator::evaluateVarUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                             unordered_map<STRING, vector<int>>& tempResults) { // a(v, _)
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
        tempResults[varName].push_back(stmtNum); // store stmtNum
        tempResults[firstArg].push_back(PKB::varTable->getVarID(varStr)); // store varID of var on LHS
    }
    if (tempResults[varName].empty() || tempResults[firstArg].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums and variables on LHS and checks if matches RHS
bool PatternEvaluator::evaluateVarExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                             STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) { // a(v, "x + y")
    STRING exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
        STRING exprStr1 = p.second;

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
bool PatternEvaluator::evaluateVarExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                           STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) { // a(v, _"x + y"_)
    STRING exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING varStr = p.first;
        STRING exprStr1 = p.second;

        if (exprStr1.find(exprStr2) != STRING::npos) {
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
bool PatternEvaluator::evaluateUnderscoreUnderscore(vector<StmtNum> stmtNums, STRING varName,
                                                    unordered_map<STRING, vector<int>>& tempResults) { // a(_, _)
    for (StmtNum& stmtNum : stmtNums) {
        tempResults[varName].push_back(stmtNum); // store all assignment stmts
    }
    if (tempResults[varName].empty()) {
        return false;
    }
    return true;
}

// Will get all assignment stmt nums that matches RHS
bool PatternEvaluator::evaluateUnderscoreExpression(vector<StmtNum> stmtNums, STRING varName,
                                                    STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) { // a(_, "x + y")
    STRING exprStr2 = parseExprToExprStr(trimQuotes(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING exprStr1 = p.second;

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
bool PatternEvaluator::evaluateUnderscoreExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName,
                                                                  STRING secondArg, unordered_map<STRING, vector<int>>& tempResults) {// a(_, _"x + y"_)
    STRING exprStr2 = parseExprToExprStr(trimQuotesUnderscore(secondArg));
    for (StmtNum& stmtNum : stmtNums) {
        pair<STRING, STRING> p = PKB::stmtTable->getAssignExpr(stmtNum);
        STRING exprStr1 = p.second;

        if (exprStr1.find(exprStr2) != STRING::npos) {
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
