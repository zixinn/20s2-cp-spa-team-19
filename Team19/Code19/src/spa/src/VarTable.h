#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class VarTable {
public:

    // Constructor for VarTable
    VarTable();

    // Returns the varID corresponding to the varName if it exists.
    // otherwise, return -1.
    int getVarID(std::string varName);

    // Returns the varName corresponding to the varID if it exists. Throws out of index exception otherwise.
    std::string getVarName(int varID);

    // returns a const reference to the vector of all variable names stored.
    std::vector<std::string> const &getAllVarNames() const;

    // returns a vector of variable names corresponding to the vector of variable IDs given.
    std::vector<std::string> convertVarIDsToNames(std::vector<int> varIDs);

    // Returns the number of variables in the table.
    int getSize();

    // stores the variable into the varNameIDMap and varNames. Returns the ID of the variable.
    int storeVarName(std::string varName);

    // Returns true if the variable has already been added before; otherwise, returns false.
    bool hasVar(std::string varName);

private:
    // A map that stores variable name as key and ID as value.
    std::unordered_map<std::string, int> varNameIDMap;

    // A vector that stores all the variable names such that the indexes are the ID of the variable.
    std::vector<std::string> varNames;

};