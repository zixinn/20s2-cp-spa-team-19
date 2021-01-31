#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <string> 

std::string trim(std::string s);

std::vector<std::string> split(std::string s, std::string delimiter);

bool checkName(std::string s);

bool checkInteger(std::string s);

bool checkNameWithQuotes(std::string s);

bool checkExpression(std::string s);

bool checkExpressionWithUnderscores(std::string s);

//bool compareAst(AST* root1, AST* root2);
