#include "ProcTable.h"

ProcTable::ProcTable() = default;

ProcID ProcTable::getProcID(STRING procName) {
    if (procNameIDMap.find(procName) == procNameIDMap.end()) {
        return -1;
    } else {
        return procNameIDMap.find(procName)->second;
    }
}

STRING ProcTable::getProcName(ProcID procID) {
    try {
        return procNames.at(procID);
    } catch (out_of_range const& e) {
        cerr << "Index out of range: No procedure with ID " << procID << " in procNameIDMap." << endl;
        throw;
    }
}

pair<StmtNum, StmtNum> ProcTable::getProcRange(ProcID procID) {
    if (procStmtMap.find(procID) == procStmtMap.end()) {
        cerr << "Index out of range: No procedure with ID " << procID << " in procStmtMap." << endl;
        throw exception();
    } else {
        return procStmtMap.find(procID)->second;
    }
}

vector<STRING> const &ProcTable::getAllProcNames() const {
    return procNames;
}

vector<STRING> ProcTable::convertProcIDsToNames(vector<ProcID> procIDs) {
    vector<string> result;
    for (int id : procIDs) {
        result.push_back(getProcName(id));
    }
    return result;
}

vector<ProcID> ProcTable::getAllProcIDs() {
    vector<ProcID> result;
    for (auto &it : procNameIDMap) {
        result.push_back(it.second);
    }
    return result;
}

int ProcTable::getSize() {
    return procNames.size();
}

ProcID ProcTable::storeProcName(STRING procName) {
    if (hasProc(procName)) {
        return getProcID(procName);
    } else {
        int id = getSize();
        procNameIDMap.insert({procName, id});
        procNames.push_back(procName);
        return id;
    }
}

bool ProcTable::storeProcStmt(ProcID procID, StmtNum startStmt, StmtNum endStmt) {
    return procStmtMap.insert({procID, make_pair(startStmt, endStmt)}).second;
}

bool ProcTable::hasProc(STRING procName) {
    return procNameIDMap.find(procName) != procNameIDMap.end();
}
