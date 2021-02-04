#pragma once

#include "../AbstractAPI.h"

class ProcTable {
public:

    // Constructor for ProcTable
    ProcTable();

    // Returns the procID corresponding to the procName if it exists.
    // otherwise, return -1.
    ID getProcID(STRING procName);

    // Returns the procName corresponding to the procID if it exists. Throws exception otherwise.
    STRING getProcName(ID procID);

    // Returns the procedure's startStmt and endStmt if it exists. Throws exception otherwise.
    pair<StmtNum, StmtNum> getProcRange(ID procID);

    // returns a const reference to the vector of all procedure names stored.
    vector<STRING> const &getAllProcNames() const;

    // returns a vector of procedure names corresponding to the vector of procedure IDs given.
    vector<STRING> convertProcIDsToNames(vector<ID> procIDs);

    // Returns the number of procedures in the table.
    int getSize();

    // stores the procedure into the procNameIDMap and procNames. Returns the ID of the procedure.
    ID storeProcName(STRING procName);

    // stores the procedure that spans from startStmt to endStmt into the ProcStmtMap. Returns true if insertion took place.
    bool storeProcStmt(ID procID, StmtNum startStmt, StmtNum endStmt);

    // Returns true if the procedure has already been added before; otherwise, returns false.
    bool hasProc(STRING procName);

private:
    // A map that stores procedure name as key and ID as value.
    unordered_map<STRING, ID> procNameIDMap;

    // A vector that stores all the procedure names such that the indexes are the ID of the procedure.
    vector<STRING> procNames;

    // A map that stores procedure id as key, and <startStmt, endStmt> as value.
    unordered_map<ID, pair<StmtNum, StmtNum> > procStmtMap;

};

