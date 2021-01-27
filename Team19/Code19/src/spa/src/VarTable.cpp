#pragma once

#include <iostream>
#include "VarTable.h"


VarTable::VarTable() {

}

int VarTable::getVarID(std::string varName) {
    if (varNameIDMap.find(varName) == varNameIDMap.end()) {
        return -1;
    } else {
        return varNameIDMap.find(varName)->second;
    }
}

std::string VarTable::getVarName(int varID) {
    try {
        return varNames.at(varID);
    } catch (std::out_of_range const& e) {
        std::cout << "Index out of range: No variable with ID " << varID << " in VarNameIDMap." << std::endl;
        throw;
    }
}

std::vector<std::string> const &VarTable::getAllVarNames() const {
    return varNames;
}

std::vector<std::string> VarTable::convertVarIDsToNames(std::vector<int> varIDs) {
    std::vector<std::string> varNames;
    for (int id : varIDs) {
        varNames.push_back(getVarName(id));
    }
    return varNames;
}

int VarTable::getSize() {
    return varNames.size();
}

int VarTable::storeVarName(std::string varName) {
    if (hasVar(varName)) {
        return getVarID(varName);
    } else {
        int id = getSize();
        varNameIDMap.insert({varName, id});
        varNames.push_back(varName);
        return id;
    }
}

bool VarTable::hasVar(std::string varName) {
    return varNameIDMap.find(varName) != varNameIDMap.end();
}

