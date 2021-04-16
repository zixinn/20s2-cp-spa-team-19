#pragma once

#include "../AbstractAPI.h"

class VarTable {
public:
    // Constructor for VarTable
    VarTable();

    // Returns the varID corresponding to the varName if it exists.
    // otherwise, return -1.
    VarID getVarID(STRING varName);

    // Returns the varName corresponding to the varID if it exists. Throws out of range exception otherwise.
    STRING getVarName(VarID varID);

    // returns a const reference to the vector of all variable names stored.
    vector<STRING> const &getAllVarNames() const;

    // returns a vector of variable names corresponding to the vector of variable IDs given.
    vector<STRING> convertVarIDsToNames(vector<VarID> varIDs);

    // returns the vector of variable IDs.
    vector<VarID> getAllVarIDs();

    // Returns the number of variables in the table.
    int getSize();

    // stores the variable into the varNameIDMap and varNames. Returns the ID of the variable.
    VarID storeVarName(STRING varName);

    // Returns true if the variable has already been added before; otherwise, returns false.
    bool hasVar(STRING varName);

private:
    // A map that stores variable name as key and ID as value.
    unordered_map<STRING, VarID> varNameIDMap;

    // A vector that stores all the variable names such that the indexes are the ID of the variable.
    vector<STRING> varNames;

};