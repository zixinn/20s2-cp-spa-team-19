#pragma once

#include "../AbstractAPI.h"

class StmtLstTable {
public:
    // Constructor for StmtLstTable
    StmtLstTable();

    // Returns a const reference to the vector of the first statement numbers of all the stmtLsts stored.
    vector<StmtNum> const &getAllStmtLsts() const;

    // Returns the number of stmtLsts stored.
    int getSize();

    // Returns true if the firstStmtNum has already been added before (stmtLst is stored); otherwise, returns false.
    bool hasStmtLst(StmtNum firstStmtNum);

    //stores <firstStmtNum> into StmtLsts.
    // Returns true if the information is added successfully, returns false otherwise.
    bool storeStmtLst(StmtNum firstStmtNum);

private:
    vector<StmtNum> stmtLsts;
};
