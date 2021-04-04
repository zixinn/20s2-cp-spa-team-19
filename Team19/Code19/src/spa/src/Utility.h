#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "AbstractAPI.h"

// A collection of utility functions used in SPA

// Trims leading and trailing spaces from string s
STRING trim(STRING s);

// Splits string s by the given delimiter and returns vector containing strings split
// Strings in vector will be trimmed
std::vector<STRING> split(STRING s, STRING delimiter);

// Checks LETTER(LETTER | DIGIT)*
// Returns true if name is valid, false otherwise
bool checkName(STRING s);

// Checks DIGIT+
// Returns true if integer is valid, false otherwise
bool checkInteger(STRING s);
