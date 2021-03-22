#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Trims leading and trailing spaces from string s
std::string trim(std::string s);

// Splits string s by the given delimiter and returns vector containing strings split
// Strings in vector will be trimmed
std::vector<std::string> split(std::string s, std::string delimiter);

// Checks LETTER(LETTER | DIGIT)*
// Returns true if name is valid, false otherwise
bool checkName(std::string s);

// Checks DIGIT+
// Returns true if integer is valid, false otherwise
bool checkInteger(std::string s);
