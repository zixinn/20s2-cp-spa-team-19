#include "StmtTable.h"

StmtTable::StmtTable() = default;

ast::Stmt StmtTable::getStmtNode(StmtNum stmtNum) {
    if (stmtASTMap.find(stmtNum) == stmtASTMap.end()) {
        std::cerr << "No statement with StmtNum " << stmtNum << " stored in stmtASTMap." << std::endl;
        throw std::exception();
    } else {
        return stmtASTMap.find(stmtNum)->second;
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

bool StmtTable::storeStmt(StmtNum stmtNum, ast::Stmt stmtNode, STRING type) {
    bool inserted = stmtASTMap.insert({stmtNum, stmtNode}).second;

    if (!inserted) {
        // insertion did not take place. Don't need to insert into the vectors of corresponding type
        return inserted;
    }

    if (type == ASSIGN) {
        assignStmtNums.push_back(stmtNum);
    } else if (type == READ) {
        readStmtNums.push_back(stmtNum);
    } else if (type == PRINT) {
        printStmtNums.push_back(stmtNum);
    } else if (type == CALL) {
        callStmtNums.push_back(stmtNum);
    } else if (type == WHILE) {
        whileStmtNums.push_back(stmtNum);
    } else if (type == IF) {
        ifStmtNums.push_back(stmtNum);
    } else {
        // Statement has already been checked and must be one of the above types.
        assert(false);
    }

    stmtNums.push_back(stmtNum);
    return inserted;
}

bool StmtTable::hasStmt(StmtNum stmtNum) {
    return stmtASTMap.find(stmtNum) != stmtASTMap.end();;
}
