#include "StmtTable.h"
// added by SP, for some reason cant run without this, but PKB works fine
#include <assert.h>

StmtTable::StmtTable() = default;

bool StmtTable::isIfStmtWithControlVar(StmtNum stmtNum, ID controlVarID) {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        return false;
    }
    return ifPatternsMap.find(stmtNum)->second == controlVarID;
}

bool StmtTable::isWhileStmtWithControlVar(StmtNum stmtNum, ID controlVarID) {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        return false;
    }
    return whilePatternsMap.find(stmtNum)->second == controlVarID;
}

ast::Stmt* StmtTable::getStmtNode(StmtNum stmtNum) {
    if (stmtASTMap.find(stmtNum) == stmtASTMap.end()) {
        std::cerr << "No statement with StmtNum " << stmtNum << " stored in stmtASTMap." << std::endl;
        throw std::exception();
    } else {
        return stmtASTMap.find(stmtNum)->second;
    }
}

unordered_set<StmtNum> const &StmtTable::getIfStmtsWithControlVar(ID controlVarID) const {
    if (reverseIfPatternsMap.find(controlVarID) == reverseIfPatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reverseIfPatternsMap.find(controlVarID)->second;
    }
}

unordered_set<StmtNum> const &StmtTable::getWhileStmtsWithControlVar(ID controlVarID) const {
    if (reverseWhilePatternsMap.find(controlVarID) == reverseWhilePatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reverseWhilePatternsMap.find(controlVarID)->second;
    }
}

ID StmtTable::getControlVarOfIfStmt(StmtNum stmtNum) {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        return -1;
    } else {
        return ifPatternsMap.find(stmtNum)->second;
    }
}

ID StmtTable::getControlVarOfWhileStmt(StmtNum stmtNum) {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        return -1;
    } else {
        return whilePatternsMap.find(stmtNum)->second;
    }
}

pair<vector<StmtNum>, vector<ID> > StmtTable::getAllIfPatterns() {
    vector<StmtNum> stmtNums;
    vector<ID> varIDs;
    for (auto &it : ifPatternsMap) {
        stmtNums.push_back(it.first);
        varIDs.push_back(it.second);
    }
    return make_pair(stmtNums, varIDs);
}

pair<vector<StmtNum>, vector<ID> > StmtTable::getAllWhilePatterns() {
    vector<StmtNum> stmtNums;
    vector<ID> varIDs;
    for (auto &it : whilePatternsMap) {
        stmtNums.push_back(it.first);
        varIDs.push_back(it.second);
    }
    return make_pair(stmtNums, varIDs);
}

pair<STRING, STRING> StmtTable::getAssignExpr(StmtNum stmtNum) {
    if (assignExprMap.find(stmtNum) == assignExprMap.end()) {
        std::cerr << "No statement with StmtNum " << stmtNum << " stored in assignExprMap." << std::endl;
        throw std::exception();
    } else {
        return assignExprMap.find(stmtNum)->second;
    }
}

vector<StmtNum> const &StmtTable::getAllAssignStmtNums() const {
    return assignStmtNums;
}

vector<StmtNum> const &StmtTable::getAllReadStmtNums() const {
    return readStmtNums;
}

vector<StmtNum> const &StmtTable::getAllPrintStmtNums() const {
    return printStmtNums;
}

vector<StmtNum> const &StmtTable::getAllCallStmtNums() const {
    return callStmtNums;
}

vector<StmtNum> const &StmtTable::getAllWhileStmtNums() const {
    return whileStmtNums;
}

vector<StmtNum> const &StmtTable::getAllIfStmtNums() const {
    return ifStmtNums;
}

vector<StmtNum> const &StmtTable::getAllStmtNums() const {
    return stmtNums;
}

int StmtTable::getSize() {
    return stmtNums.size();
}

int StmtTable::getIfPatternsSize() {
    return ifPatternsMap.size();
}

int StmtTable::getWhilePatternsSize() {
    return whilePatternsMap.size();
}

bool StmtTable::storeStmt(StmtNum stmtNum, ast::Stmt *stmtNode, STRING type) {
    bool inserted = stmtASTMap.insert({stmtNum, stmtNode}).second;

    if (!inserted) {
        // insertion did not take place. Don't need to insert into the vectors of corresponding type
        return inserted;
    }

    if (type == ASSIGN_) {
        assignStmtNums.push_back(stmtNum);
    } else if (type == READ_) {
        readStmtNums.push_back(stmtNum);
    } else if (type == PRINT_) {
        printStmtNums.push_back(stmtNum);
    } else if (type == CALL_) {
        callStmtNums.push_back(stmtNum);
    } else if (type == WHILE_) {
        whileStmtNums.push_back(stmtNum);
    } else if (type == IF_) {
        ifStmtNums.push_back(stmtNum);
    } else {
        // Statement has already been checked and must be one of the above types.
        assert(false);
    }

    stmtNums.push_back(stmtNum);
    return inserted;
}

bool StmtTable::storeAssignExpr(StmtNum stmtNum, STRING varName, STRING expr) {
    return assignExprMap.insert({stmtNum, make_pair(varName, expr)}).second;
}

bool StmtTable::storeIfPattern(StmtNum stmtNum, ID controlVarID) {
    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end() || whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        return false;
    }

    ifPatternsMap.insert({stmtNum, controlVarID});

    auto it = reverseIfPatternsMap.find(controlVarID);
    if (it == reverseIfPatternsMap.end()) {
        reverseIfPatternsMap.insert({controlVarID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool StmtTable::storeWhilePattern(StmtNum stmtNum, ID controlVarID) {
    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end() || whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        return false;
    }

    whilePatternsMap.insert({stmtNum, controlVarID});

    auto it = reverseWhilePatternsMap.find(controlVarID);
    if (it == reverseWhilePatternsMap.end()) {
        reverseWhilePatternsMap.insert({controlVarID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool StmtTable::hasStmt(StmtNum stmtNum) {
    return stmtASTMap.find(stmtNum) != stmtASTMap.end();
}
