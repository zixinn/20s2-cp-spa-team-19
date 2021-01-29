#pragma once

#include "../AbstractAPI.h"

class VarTable {
public:

    // Constructor for VarTable
    VarTable();

    // Returns the varID corresponding to the varName if it exists.
    // otherwise, return -1.
    ID getVarID(STRING varName);

    // Returns the varName corresponding to the varID if it exists. Throws out of index exception otherwise.
    STRING getVarName(ID varID);

    // returns a const reference to the vector of all variable names stored.
    vector<STRING> const &getAllVarNames() const;

    // returns a vector of variable names corresponding to the vector of variable IDs given.
    vector<STRING> convertVarIDsToNames(vector<ID> varIDs);

    // Returns the number of variables in the table.
    int getSize();

    // stores the variable into the varNameIDMap and varNames. Returns the ID of the variable.
    ID storeVarName(STRING varName);

    // Returns true if the variable has already been added before; otherwise, returns false.
    bool hasVar(STRING varName);

private:
    // A map that stores variable name as key and ID as value.
    unordered_map<STRING, ID> varNameIDMap;

    // A vector that stores all the variable names such that the indexes are the ID of the variable.
    vector<STRING> varNames;

};