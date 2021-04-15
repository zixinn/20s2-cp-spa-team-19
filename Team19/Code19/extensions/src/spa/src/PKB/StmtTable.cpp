#include "StmtTable.h"
#include <assert.h>

StmtTable::StmtTable() = default;

bool StmtTable::isIfStmtWithControlVar(StmtNum stmtNum, VarID controlVarID) {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        return false;
    }
    return ifPatternsMap.find(stmtNum)->second.find(controlVarID) != ifPatternsMap.find(stmtNum)->second.end();
}

bool StmtTable::isWhileStmtWithControlVar(StmtNum stmtNum, VarID controlVarID) {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        return false;
    }
    return whilePatternsMap.find(stmtNum)->second.find(controlVarID) != whilePatternsMap.find(stmtNum)->second.end();
}

ast::Stmt* StmtTable::getStmtNode(StmtNum stmtNum) {
    if (stmtASTMap.find(stmtNum) == stmtASTMap.end()) {
        cerr << "No statement with StmtNum " << stmtNum << " stored in stmtASTMap." << endl;
        throw exception();
    } else {
        return stmtASTMap.find(stmtNum)->second;
    }
}

unordered_set<StmtNum> const &StmtTable::getIfStmtsWithControlVar(VarID controlVarID) const {
    if (reverseIfPatternsMap.find(controlVarID) == reverseIfPatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reverseIfPatternsMap.find(controlVarID)->second;
    }
}

unordered_set<StmtNum> const &StmtTable::getWhileStmtsWithControlVar(VarID controlVarID) const {
    if (reverseWhilePatternsMap.find(controlVarID) == reverseWhilePatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reverseWhilePatternsMap.find(controlVarID)->second;
    }
}

const unordered_set<VarID> & StmtTable::getControlVarsOfIfStmt(StmtNum stmtNum) const {
    if (ifPatternsMap.find(stmtNum) == ifPatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return ifPatternsMap.find(stmtNum)->second;
    }
}

const unordered_set<VarID> & StmtTable::getControlVarsOfWhileStmt(StmtNum stmtNum) const {
    if (whilePatternsMap.find(stmtNum) == whilePatternsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return whilePatternsMap.find(stmtNum)->second;
    }
}

VarID StmtTable::getReadVariableOfStmt(StmtNum stmtNum) {
    auto it = readVariablesMap.find(stmtNum);
    if (it == readVariablesMap.end()) {
        return -1;
    }
    return it->second;
}

VarID StmtTable::getPrintVariableOfStmt(StmtNum stmtNum) {
    auto it = printVariablesMap.find(stmtNum);
    if (it == printVariablesMap.end()) {
        return -1;
    }
    return it->second;
}

unordered_set<StmtNum> const &StmtTable::getStmtNumsOfReadWithVar(VarID readVarID) const {
    if (reverseReadVariablesMap.find(readVarID) == reverseReadVariablesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reverseReadVariablesMap.find(readVarID)->second;
    }
}
unordered_set<StmtNum> const &StmtTable::getStmtNumsOfPrintWithVar(VarID printVarID) const {
    if (reversePrintVariablesMap.find(printVarID) == reversePrintVariablesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    } else {
        return reversePrintVariablesMap.find(printVarID)->second;
    }
}

pair<vector<StmtNum>, vector<VarID>> StmtTable::getAllIfPatterns() {
    vector<StmtNum> first;
    vector<VarID> varIDs;
    for (auto &it : ifPatternsMap) {
        for (VarID varID : it.second) {
            first.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(first, varIDs);
}

pair<vector<StmtNum>, vector<VarID>> StmtTable::getAllWhilePatterns() {
    vector<StmtNum> first;
    vector<VarID> varIDs;
    for (auto &it : whilePatternsMap) {
        for (VarID varID : it.second) {
            first.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(first, varIDs);
}

pair<STRING, STRING> StmtTable::getAssignExpr(StmtNum stmtNum) {
    if (assignExprMap.find(stmtNum) == assignExprMap.end()) {
        cerr << "No statement with StmtNum " << stmtNum << " stored in assignExprMap." << endl;
        throw exception();
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

pair<StmtNum, set<StmtNum>> StmtTable::getIfStmtRange(StmtNum stmtNum) {
    if (ifStmtMap.find(stmtNum) == ifStmtMap.end()) {
        cerr << "Stmt " << stmtNum << " is not an if stmt." << endl;
        throw exception();
    } else {
        return ifStmtMap.find(stmtNum)->second;
    }
}

pair<StmtNum, set<StmtNum>> StmtTable::getElseStmtRange(StmtNum stmtNum) {
    if (elseStmtMap.find(stmtNum) == elseStmtMap.end()) {
        cerr << "Stmt " << stmtNum << " is not an if stmt." << endl;
        throw exception();
    } else {
        return elseStmtMap.find(stmtNum)->second;
    }
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

bool StmtTable::storeReadVariableForStmt(StmtNum stmtNum, VarID readVarID) {
    if (readVariablesMap.find(stmtNum) != readVariablesMap.end()) {
        return false;
    }
    readVariablesMap.insert({stmtNum, readVarID});

    auto it = reverseReadVariablesMap.find(readVarID);
    if (it == reverseReadVariablesMap.end()) {
        reverseReadVariablesMap.insert({readVarID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool StmtTable::storePrintVariableForStmt(StmtNum stmtNum, VarID printVarID) {
    if (printVariablesMap.find(stmtNum) != printVariablesMap.end()) {
        return false;
    }
    printVariablesMap.insert({stmtNum, printVarID});

    auto it = reversePrintVariablesMap.find(printVarID);
    if (it == reversePrintVariablesMap.end()) {
        reversePrintVariablesMap.insert({printVarID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool StmtTable::storeIfPattern(StmtNum stmtNum, VarID controlVarID) {
    if (whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        return false;
    }

    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end()) {
        if (ifPatternsMap.find(stmtNum)->second.find(controlVarID) != ifPatternsMap.find(stmtNum)->second.end()) {
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

bool StmtTable::storeWhilePattern(StmtNum stmtNum, VarID controlVarID) {
    if (ifPatternsMap.find(stmtNum) != ifPatternsMap.end()) {
        return false;
    }

    if (whilePatternsMap.find(stmtNum) != whilePatternsMap.end()) {
        if (whilePatternsMap.find(stmtNum)->second.find(controlVarID) != whilePatternsMap.find(stmtNum)->second.end()) {
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

bool StmtTable::storeIfStmt(StmtNum ifStmtNum, StmtNum startStmt, const set<StmtNum>& endStmt) {
    return ifStmtMap.insert({ifStmtNum, make_pair(startStmt, endStmt)}).second;
}

bool StmtTable::storeElseStmt(StmtNum elseStmtNum, StmtNum startStmt, const set<StmtNum>& endStmt) {
    return elseStmtMap.insert({elseStmtNum, make_pair(startStmt, endStmt)}).second;
}

bool StmtTable::hasStmt(StmtNum stmtNum) {
    return stmtASTMap.find(stmtNum) != stmtASTMap.end();
}
