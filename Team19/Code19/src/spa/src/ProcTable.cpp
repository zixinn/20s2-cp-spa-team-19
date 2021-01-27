#pragma once

#include <iostream>
#include "ProcTable.h"

ProcTable::ProcTable() = default;

int ProcTable::getProcID(std::string procName) {
    if (procNameIDMap.find(procName) == procNameIDMap.end()) {
        return -1;
    } else {
        return procNameIDMap.find(procName)->second;
    }
}

std::string ProcTable::getProcName(int procID) {
    try {
        return procNames.at(procID);
    } catch (std::out_of_range const& e) {
        std::cout << "Index out of range: No procedure with ID " << procID << " in procNameIDMap." << std::endl;
        throw;
    }
}

std::pair<int, int> ProcTable::getProcRange(int procID) {
    if (procStmtMap.find(procID) == procStmtMap.end()) {
        std::cout << "Index out of range: No procedure with ID " << procID << " in procStmtMap." << std::endl;
        throw std::out_of_range("ProcID not found in procStmtMap.");
    } else {
        return procStmtMap.find(procID)->second;
    }
}

std::vector<std::string> const &ProcTable::getAllProcNames() const {
    return procNames;
}

std::vector<std::string> ProcTable::convertProcIDsToNames(std::vector<int> procIDs) {
    std::vector<std::string> result;
    for (int id : procIDs) {
        result.push_back(getProcName(id));
    }
    return result;
}

int ProcTable::getSize() {
    return procNames.size();
}

int ProcTable::storeProcName(std::string procName) {
    if (hasProc(procName)) {
        return getProcID(procName);
    } else {
        int id = getSize();
        procNameIDMap.insert({procName, id});
        procNames.push_back(procName);
        return id;
    }
}

bool ProcTable::storeProcStmt(int procID, int startStmt, int endStmt) {
    return procStmtMap.insert({procID, std::make_pair(startStmt, endStmt)}).second;
}

bool ProcTable::hasProc(std::string procName) {
    return procNameIDMap.find(procName) != procNameIDMap.end();
}
