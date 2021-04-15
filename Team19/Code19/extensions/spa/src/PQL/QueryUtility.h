#pragma once

#include "../PKB/PKB.h"
#include "../SP/Lexer.h"
#include "Clause.h"

// Checks if name with quotes conforms to naming standards
// Returns true if name is valid, false otherwise
bool checkNameWithQuotes(STRING s);

// Returns true if the TokenType is an operator, false otherwise
bool isOperator(sp::Token::TokenType tokenType);

// Checks if expression is valid
// Returns true if expression is valid, false otherwise
bool checkExpression(STRING s);

// Checks if expression with underscores is valid
// Returns true if expression is valid, false otherwise
bool checkExpressionWithUnderscores(STRING s);

// Checks if the synonym has been declared in declarations
bool checkSynonymDeclared(string synonym, unordered_map<string, string> declarations);

// Returns the type of the synonym
string getArgType(string synonym, unordered_map<string, string> declarations);

// Returns stmtNum of all stmt with type synonymType or ID of all proc/var/const with type synonymType
vector<int> selectAll(string synonymType);

// Stores the intersection of allResults and allCorrectType in results, removes duplicates
// Returns true if results is non-empty, false otherwise
bool intersectSingleSynonym(vector<int> allResults, vector<int> allCorrectType, vector<int>& results);

// Function overload
// Stores the intersection of allResults and allCorrectType in results, removes duplicates
// Returns true if results is non-empty, false otherwise
bool intersectSingleSynonym(unordered_set<int> allResults, vector<int> allCorrectType, vector<int>& results);

// Stores pairs of entries of allResults such that the first entry exists in allCorrectType.first and the second entry exists in allCorrectType.second in results
// Returns true if results is non-empty, false otherwise
bool intersectDoubleSynonym(pair<vector<int>, vector<int>> allResults, pair<vector<int>, vector<int>> allCorrectType, pair<vector<int>, vector<int>>& results);

// Projects only synonyms in the set toProject from results, removing duplicates
void project(unordered_set<string> toProject, unordered_map<string, vector<int>>& results);

// Returns the size of the relation for the clause
int getSize(Clause clause, unordered_map<string, string>& declarations);
