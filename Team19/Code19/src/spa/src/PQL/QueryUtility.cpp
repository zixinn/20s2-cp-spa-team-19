#include "QueryUtility.h"

using namespace std;

bool checkNameWithQuotes(string s) {
    if (s[0] != '\"' || s[s.length() - 1] != '\"') {
        return false;
    }
    return checkName(trim(s.substr(1, s.length() - 2)));
}

bool isOperator(sp::Token::TokenType tokenType) {
    return tokenType ==  sp::Token::TokenType::PLUS || tokenType ==  sp::Token::TokenType::MINUS
        || tokenType ==  sp::Token::TokenType::TIMES || tokenType ==  sp::Token::TokenType::DIV
        || tokenType ==  sp::Token::TokenType::MOD;
}

bool checkExpression(string s) {
    if (s[0] != '\"' || s[s.length() - 1] != '\"') {
        return false;
    }
    string str = trim(s.substr(1, s.length() - 2));

    vector<sp::Token> tokens;
    bool b = Lexer::tokenise(str, tokens);
    if (!b) {
        return false;
    }

    sp::Token::TokenType firstTokenType = tokens.at(0).getType();
    sp::Token::TokenType lastTokenType = tokens.at(tokens.size() - 2).getType();
    if (isOperator(firstTokenType) || isOperator(lastTokenType)) {
        return false;
    }

    int parenCount = 0;
    bool isPrevOperator = false;
    bool isPrevOperand = false;
    bool isPrevLParen = false;

    for (int i = 0; i < tokens.size() - 1; i++) {
        sp::Token::TokenType tokenType = tokens.at(i).getType();
        string tokenLiteral = tokens.at(i).getLiteral();
        if (tokenType == sp::Token::TokenType::LPAREN) {
            parenCount++;
            isPrevLParen = true;
            isPrevOperator = false;
            isPrevOperand = false;
        } else if (tokenType == sp::Token::TokenType::RPAREN) {
            if (parenCount <= 0 || isPrevOperator || isPrevLParen) {
                return false;
            }
            parenCount--;
            isPrevOperand = false;
        } else if (isOperator(tokenType)) {
            if (isPrevOperator || isPrevLParen) {
                return false;
            }
            isPrevOperator = true;
            isPrevOperand = false;
        } else if (!checkName(tokenLiteral) && !checkInteger(tokenLiteral))  {
            return false;
        } else {
            if (isPrevOperand) {
                return false;
            }
            isPrevOperator = false;
            isPrevOperand = true;
            isPrevLParen = false;
        }
    }

    if (parenCount != 0 || isPrevOperator || isPrevLParen) {
        return false;
    }
    return true;
}

bool checkExpressionWithUnderscores(string s) {
    if (s[0] != '_' || s[s.length() - 1] != '_') {
        return false;
    }
    return checkExpression(trim(s.substr(1, s.length() - 2)));
}

bool checkSynonymDeclared(string synonym, unordered_map<string, string> declarations) {
    return declarations.find(synonym) != declarations.end();
}

string getArgType(string synonym, unordered_map<string, string> declarations) {
    if (checkSynonymDeclared(synonym, declarations)) {
        return declarations[synonym];
    } else if (checkInteger(synonym)) {
        return INTEGER_;
    } else if (synonym == "_") {
        return UNDERSCORE_;
    } else if (checkNameWithQuotes(synonym)) {
        return NAME_;
    } else if (checkExpression(synonym)) {
        return EXPRESSION_;
    } else if (checkExpressionWithUnderscores(synonym)) {
        return EXPRESSIONWITHUNDERSCORE_;
    } else {
        return "";
    }
}

vector<int> selectAll(string synonymType) {
    vector<int> res;
    if (synonymType == PROCEDURE_) {
        return PKB::procTable->getAllProcIDs();
    } else if (synonymType == STMT_ || synonymType == PROGLINE_) {
        return PKB::stmtTable->getAllStmtNums();
    } else if (synonymType == READ_) {
        return PKB::stmtTable->getAllReadStmtNums();
    } else if (synonymType == PRINT_) {
        return PKB::stmtTable->getAllPrintStmtNums();
    } else if (synonymType == ASSIGN_) {
        return PKB::stmtTable->getAllAssignStmtNums();
    } else if (synonymType == CALL_) {
        return PKB::stmtTable->getAllCallStmtNums();
    } else if (synonymType == WHILE_) {
        return PKB::stmtTable->getAllWhileStmtNums();
    } else if (synonymType == IF_) {
        return PKB::stmtTable->getAllIfStmtNums();
    } else if (synonymType == VARIABLE_) {
        return PKB::varTable->getAllVarIDs();
    } else if (synonymType == CONSTANT_) {
        return PKB::constTable->getAllConsts();
    }
    return res;
}

bool intersectSingleSynonym(vector<int> allResults, vector<int> allCorrectType, vector<int>& results) {
    unordered_set<int> set(allResults.begin(), allResults.end());
    allResults.assign(set.begin(), set.end());
    vector<int> res;
    int size = allResults.size();
    for (int i = 0; i < size; i++) {
        if (find(allCorrectType.begin(), allCorrectType.end(), allResults.at(i)) != allCorrectType.end()) {
            res.push_back(allResults.at(i));
        }
    }
    results = res;
    return !res.empty();
}

bool intersectSingleSynonym(unordered_set<int> allResults, vector<int> allCorrectType, vector<int>& results) {
    vector<int> allResultsVector;
    allResultsVector.assign(allResults.begin(), allResults.end());
    vector<int> res;
    int size = allResultsVector.size();
    for (int i = 0; i < size; i++) {
        if (find(allCorrectType.begin(), allCorrectType.end(), allResultsVector.at(i)) != allCorrectType.end()) {
            res.push_back(allResultsVector.at(i));
        }
    }
    results = res;
    return !res.empty();
}

bool intersectDoubleSynonym(pair<vector<int>, vector<int>> allResults, pair<vector<int>, vector<int>> allCorrectType, pair<vector<int>, vector<int>>& results) {
    pair<vector<int>, vector<int>> res;
    int size = allResults.first.size();
    for (int i = 0; i < size; i++) {
        if (find(allCorrectType.first.begin(), allCorrectType.first.end(), allResults.first.at(i)) != allCorrectType.first.end()
            && find(allCorrectType.second.begin(), allCorrectType.second.end(), allResults.second.at(i)) != allCorrectType.second.end()) {
            res.first.push_back(allResults.first.at(i));
            res.second.push_back(allResults.second.at(i));
        }
    }
    results = res;
    return !res.first.empty();
}

void project(unordered_set<string> toProject, unordered_map<string, vector<int>>& results) {
    // find entries to be removed
    unordered_set<string> toRemove;
    for (auto it : results) {
        string synonym = it.first;
        if (toProject.find(synonym) == toProject.end()) {
            toRemove.insert(synonym);
        }
    }

    // project
    for (auto it : toRemove) {
        results.erase(it);
    }
    if (results.empty()) {
        return;
    }

    // remove duplicates
    unordered_map<string, vector<int>> newResults;
    for (auto it : results) {
        newResults.insert(make_pair(it.first, vector<int>{}));
    }
    unordered_set<string> set;
    int numRows = results.begin()->second.size();
    for (int i = 0; i < numRows; i++) {
        string s;
        for (auto it : results) {
            s += to_string(it.second.at(i)) + " ";
        }
        if (set.find(s) == set.end()) { // row is unique
            set.insert(s);
            for (auto it : results) { // insert row into table
                newResults.find(it.first)->second.push_back(it.second.at(i));
            }
        }
    }
    results = newResults;
}

int getSize(Clause clause, unordered_map<string, string>& declarations) {
    string rel = clause.getRel();
    if (rel == "Follows") {
        return PKB::follows->getFollowsSize();
    } else if (rel == "Follows*") {
        return PKB::follows->getFollowsStarSize();
    } else if (rel == "Parent") {
        return PKB::parent->getParentSize();
    } else if (rel == "Parent*") {
        return PKB::parent->getParentStarSize();
    } else if (rel == "Uses") {
        string argType = getArgType(clause.getArgs().at(0), declarations);
        if (argType == NAME_ || argType == PROCEDURE_) {
            return PKB::uses->getProcSize();
        } else {
            return PKB::uses->getStmtSize();
        }
    } else if (rel == "Modifies") {
        string argType = getArgType(clause.getArgs().at(0), declarations);
        if (argType == NAME_ || argType == PROCEDURE_) {
            return PKB::modifies->getProcSize();
        } else {
            return PKB::modifies->getStmtSize();
        }
    } else if (rel == "Calls") {
        return PKB::calls->getCallsSize();
    } else if (rel == "Calls*") {
        return PKB::calls->getCallsStarSize();
    } else if (rel == "Next") {
        return PKB::next->getNextSize();
    } else if (rel == "Next*") {
        return PKB::next->getNextStarSize();
    } else if (rel == "Affects") {
        return PKB::affects->getAffectsSize();
    } else if (rel == "Affects*") {
        return PKB::affects->getAffectsStarSize();
    } else if (rel == "") { // with clause
        if (clause.getNumOfKnown() > 0) {
            return 0;
        }
        return max(max(PKB::procTable->getSize(), PKB::varTable->getSize()),
                   max(PKB::constTable->getSize(), PKB::stmtTable->getSize()));
    } else { // pattern clause
        string argType = getArgType(rel, declarations);
        if (argType == ASSIGN_) {
            return PKB::stmtTable->getAllAssignStmtNums().size();
        } else if (argType == WHILE_) {
            return PKB::stmtTable->getAllWhileStmtNums().size();
        } else { // argType == IF_
            return PKB::stmtTable->getAllIfStmtNums().size();
        }
    }
}
