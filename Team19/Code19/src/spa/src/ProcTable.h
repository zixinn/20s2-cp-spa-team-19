#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class ProcTable {
public:

    // Constructor for ProcTable
    ProcTable();

    // Returns the procID corresponding to the procName if it exists.
    // otherwise, return -1.
    int getProcID(std::string procName);

    // Returns the procName corresponding to the procID if it exists. Throws out of index exception otherwise.
    std::string getProcName(int procID);

    // Returns the procedure's startStmt and endStmt if it exists. Throws out of index exception otherwise.
    std::pair<int, int> getProcRange(int procID);

    // returns a const reference to the vector of all procedure names stored.
    std::vector<std::string> const &getAllProcNames() const;

    // returns a vector of procedure names corresponding to the vector of procedure IDs given.
    std::vector<std::string> convertProcIDsToNames(std::vector<int> procIDs);

    // Returns the number of procedures in the table.
    int getSize();

    // stores the procedure into the procNameIDMap and procNames. Returns the ID of the procedure.
    int storeProcName(std::string procName);

    // stores the procedure that spans from startStmt to endStmt into the ProcStmtMap. Returns true if insertion took place.
    bool storeProcStmt(int procID, int startStmt, int endStmt);

    // Returns true if the procedure has already been added before; otherwise, returns false.
    bool hasProc(std::string procName);

private:
    // A map that stores procedure name as key and ID as value.
    std::unordered_map<std::string, int> procNameIDMap;

    // A vector that stores all the procedure names such that the indexes are the ID of the procedure.
    std::vector<std::string> procNames;

    // A map that stores procedure id as key, and <startStmt, endStmt> as value.
    std::unordered_map<int, std::pair<int, int> > procStmtMap;

};

