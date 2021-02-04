#include <iostream>
#include "ProcTable.h"

ProcTable::ProcTable() = default;

ID ProcTable::getProcID(STRING procName) {
    if (procNameIDMap.find(procName) == procNameIDMap.end()) {
        return -1;
    } else {
        return procNameIDMap.find(procName)->second;
    }
}

STRING ProcTable::getProcName(ID procID) {
    try {
        return procNames.at(procID);
    } catch (std::out_of_range const& e) {
        std::cerr << "Index out of range: No procedure with ID " << procID << " in procNameIDMap." << std::endl;
        throw;
    }
}

pair<StmtNum, StmtNum> ProcTable::getProcRange(ID procID) {
    if (procStmtMap.find(procID) == procStmtMap.end()) {
        std::cerr << "Index out of range: No procedure with ID " << procID << " in procStmtMap." << std::endl;
        throw std::exception();
    } else {
        return procStmtMap.find(procID)->second;
    }
}

vector<STRING> const &ProcTable::getAllProcNames() const {
    return procNames;
}

vector<STRING> ProcTable::convertProcIDsToNames(vector<ID> procIDs) {
    std::vector<std::string> result;
    for (int id : procIDs) {
        result.push_back(getProcName(id));
    }
    return result;
}

int ProcTable::getSize() {
    return procNames.size();
}

ID ProcTable::storeProcName(STRING procName) {
    if (hasProc(procName)) {
        return getProcID(procName);
    } else {
        int id = getSize();
        procNameIDMap.insert({procName, id});
        procNames.push_back(procName);
        return id;
    }
}

bool ProcTable::storeProcStmt(ID procID, StmtNum startStmt, StmtNum endStmt) {
    return procStmtMap.insert({procID, std::make_pair(startStmt, endStmt)}).second;
}

bool ProcTable::hasProc(STRING procName) {
    return procNameIDMap.find(procName) != procNameIDMap.end();
}
