#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "../AbstractAPI.h"
#include "../Utility.h"
#include "../PKB/PKB.h"
#include "../SP/Lexer.h"

bool checkNameWithQuotes(std::string s);

bool isOperator(sp::Token::TokenType tokenType);

bool checkExpression(std::string s);

bool checkExpressionWithUnderscores(std::string s);

bool checkSynonymDeclared(string synonym, unordered_map<string, string> declarations);

string getArgType(string synonym, unordered_map<string, string> declarations);

vector<int> selectAll(string synonymType);

bool intersectSingleSynonym(vector<int> allResults, vector<int> allCorrectType, vector<int>& results);

bool intersectSingleSynonym(unordered_set<int> allResults, vector<int> allCorrectType, vector<int>& results);

bool intersectDoubleSynonym(pair<vector<int>, vector<int>> allResults, pair<vector<int>, vector<int>> allCorrectType, pair<vector<int>, vector<int>>& results);
