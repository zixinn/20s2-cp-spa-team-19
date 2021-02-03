#include "StmtLstTable.h"

StmtLstTable::StmtLstTable() = default;

vector<StmtNum> const &StmtLstTable::getAllStmtLsts() const {
    return stmtLsts;
}

int StmtLstTable::getSize() {
    return stmtLsts.size();
}

bool StmtLstTable::hasStmtLst(StmtNum firstStmtNum) {
    return find(stmtLsts.begin(), stmtLsts.end(), firstStmtNum) != stmtLsts.end();
}

bool StmtLstTable::storeStmtLst(StmtNum firstStmtNum) {
    if (hasStmtLst(firstStmtNum)) {
        return false;
    }
    stmtLsts.push_back(firstStmtNum);
    return true;
}
