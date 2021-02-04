#pragma once

#include "../AbstractAPI.h"

// Maps the values of all constants used in the SIMPLE program to their string representations.
class ConstTable {
public:
    // Constructor
    ConstTable();

    // Returns the value of the specified constant. Returns -1 if it is not found.
    CONST getConstValue(STRING const);
    // Returns a const reference to the vector of all constants stored.
    vector<CONST> const &getAllConsts() const;
    // Returns TRUE if const exists in the table; false otherwise.
    bool hasConst(STRING const);
    // Returns the number of constants stored in the table.
    int getSize();

    // Stores the constant and returns false if it already exists; returns true otherwise.
    bool storeConst(STRING const);

private:
    unordered_map<STRING, CONST> constMap;
    vector<CONST> consts;
};
