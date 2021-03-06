#include <iostream>
#include "VarTable.h"

VarTable::VarTable() = default;

VarID VarTable::getVarID(STRING varName) {
    if (varNameIDMap.find(varName) == varNameIDMap.end()) {
        return -1;
    } else {
        return varNameIDMap.find(varName)->second;
    }
}

STRING VarTable::getVarName(VarID varID) {
    try {
        return varNames.at(varID);
    } catch (out_of_range const& e) {
        cerr << "Index out of range: No variable with ID " << varID << " in VarNameIDMap." << endl;
        throw;
    }
}

vector<STRING> const &VarTable::getAllVarNames() const {
    return varNames;
}

vector<STRING> VarTable::convertVarIDsToNames(vector<VarID> varIDs) {
    vector<string> result;
    for (int id : varIDs) {
        result.push_back(getVarName(id));
    }
    return result;
}

vector<VarID> VarTable::getAllVarIDs() {
    vector<VarID> result;
    for (auto &it : varNameIDMap) {
        result.push_back(it.second);
    }
    return result;
}

int VarTable::getSize() {
    return varNames.size();
}

VarID VarTable::storeVarName(STRING varName) {
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

