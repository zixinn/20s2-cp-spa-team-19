#include "StmtTable.h"
// added by SP, for some reason cant run without this, but PKB works fine
#include <assert.h>

StmtTable::StmtTable() = default;

bool StmtTable::isIfStmtWithControlVar(StmtNum stmtNum, ID controlVarID) {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        return false;
    }
    return ifPatternsMap.find(stmtNum)->second.find(controlVarID) != ifPatternsMap.find(stmtNum)->second.end();
}

bool StmtTable::isWhileStmtWithControlVar(StmtNum stmtNum, ID controlVarID) {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        return false;
    }
    return whilePatternsMap.find(stmtNum)->second.find(controlVarID) != whilePatternsMap.find(stmtNum)->second.end();
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

unordered_set<ID> const &StmtTable::getControlVarsOfIfStmt(StmtNum stmtNum) const {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return ifPatternsMap.find(stmtNum)->second;
    }
}

unordered_set<ID> const &StmtTable::getControlVarsOfWhileStmt(StmtNum stmtNum) const {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return whilePatternsMap.find(stmtNum)->second;
    }
}

pair<vector<StmtNum>, vector<ID> > StmtTable::getAllIfPatterns() {
    vector<StmtNum> first;
    vector<ID> varIDs;
    for (auto &it : ifPatternsMap) {
        for (ID varID : it.second) {
            first.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(first, varIDs);
}

pair<vector<StmtNum>, vector<ID> > StmtTable::getAllWhilePatterns() {
    vector<StmtNum> first;
    vector<ID> varIDs;
    for (auto &it : whilePatternsMap) {
        for (ID varID : it.second) {
            first.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(first, varIDs);
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
    int cnt = 0;
    for (auto &it : ifPatternsMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int StmtTable::getWhilePatternsSize() {
    int cnt = 0;
    for (auto &it : whilePatternsMap) {
        cnt += it.second.size();
    }
    return cnt;
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
    if (whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        return false;
    }

    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end()) {
        if (ifPatternsMap.find(stmtNum)->second.find(controlVarID) != ifPatternsMap.find(stmtNum)->second.end()) {
            return false;
        }
        if (ifPatternsMap.find(stmtNum)->second.size() == 2) {
            return false;
        }
    }

    auto it = ifPatternsMap.find(stmtNum);
    if (it == ifPatternsMap.end()) {
        ifPatternsMap.insert({stmtNum, unordered_set<StmtNum>({controlVarID})});
    } else {
        it->second.insert(controlVarID);
    }

    it = reverseIfPatternsMap.find(controlVarID);
    if (it == reverseIfPatternsMap.end()) {
        reverseIfPatternsMap.insert({controlVarID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool StmtTable::storeWhilePattern(StmtNum stmtNum, ID controlVarID) {
    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end()) {
        return false;
    }

    if (whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        if (whilePatternsMap.find(stmtNum)->second.find(controlVarID) != whilePatternsMap.find(stmtNum)->second.end()) {
            return false;
        }
        if (whilePatternsMap.find(stmtNum)->second.size() == 2) {
            return false;
        }
    }

    auto it = whilePatternsMap.find(stmtNum);
    if (it == whilePatternsMap.end()) {
        whilePatternsMap.insert({stmtNum, unordered_set<StmtNum>({controlVarID})});
    } else {
        it->second.insert(controlVarID);
    }

    it = reverseWhilePatternsMap.find(controlVarID);
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
