#include <iostream>
#include "VarTable.h"


VarTable::VarTable() = default;

ID VarTable::getVarID(STRING varName) {
    if (varNameIDMap.find(varName) == varNameIDMap.end()) {
        return -1;
    } else {
        return varNameIDMap.find(varName)->second;
    }
}

STRING VarTable::getVarName(ID varID) {
    try {
        return varNames.at(varID);
    } catch (std::out_of_range const& e) {
        std::cerr << "Index out of range: No variable with ID " << varID << " in VarNameIDMap." << std::endl;
        throw;
    }
}

vector<STRING> const &VarTable::getAllVarNames() const {
    return varNames;
}

vector<STRING> VarTable::convertVarIDsToNames(vector<ID> varIDs) {
    std::vector<std::string> result;
    for (int id : varIDs) {
        result.push_back(getVarName(id));
    }
    return result;
}

int VarTable::getSize() {
    return varNames.size();
}

ID VarTable::storeVarName(STRING varName) {
    if (hasVar(varName)) {
        return getVarID(varName);
    } else {
        int id = getSize();
        varNameIDMap.insert({varName, id});
        varNames.push_back(varName);
        return id;
    }
}

bool VarTable::hasVar(STRING varName) {
    return varNameIDMap.find(varName) != varNameIDMap.end();
}

