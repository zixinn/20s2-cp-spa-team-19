#include "QueryUtility.h"

using namespace std;

bool checkSynonymDeclared(string synonym, unordered_map<string, string> declarations) {
    return declarations.find(synonym) != declarations.end();
}

// Returns the type of the synonym
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

// Returns stmtNum of all stmt with type synonymType or ID of all proc/var/const with type synonymType
vector<int> selectAll(string synonymType) {
    vector<int> res;
    if (synonymType == PROCEDURE_) {
        int size = PKB::procTable->getSize();
        for (int i = 0; i < size; i++) {
            res.push_back(i);
        }
        return res;
    } else if (synonymType == STMT_) {
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
        int size = PKB::varTable->getSize();
        for (int i = 0; i < size; i++) {
            res.push_back(i);
        }
        return res;
    } else if (synonymType == CONSTANT_) {
        return PKB::constTable->getAllConsts();
    }
    return res;
}

// Stores the intersection of allResults and allCorrectType in results
// Returns true if results is non-empty, false otherwise
bool intersectSingleSynonym(vector<int> allResults, vector<int> allCorrectType, vector<int>& results) {
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

// Stores the intersection of allResults and allCorrectType in results
// Returns true if results is non-empty, false otherwise
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
