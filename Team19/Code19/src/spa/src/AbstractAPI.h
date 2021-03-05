#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "../../autotester/src/AbstractWrapper.h"

#define PROCEDURE_ "procedure"
#define PROGLINE_ "prog_line"
#define STMTLST_ "stmtLst"
#define STMT_ "stmt"
#define READ_ "read"
#define PRINT_ "print"
#define ASSIGN_ "assign"
#define CALL_ "call"
#define WHILE_ "while"
#define IF_ "if"
#define VARIABLE_ "variable"
#define CONSTANT_ "constant"
#define INTEGER_ "integer"
#define UNDERSCORE_ "underscore"
#define NAME_ "name"
#define EXPRESSION_ "expression"
#define EXPRESSIONWITHUNDERSCORE_ "expressionwithunderscore"

using namespace std;

typedef int StmtNum;
typedef int ID;
typedef int CONST;
typedef std::string STRING;
typedef std::vector<StmtNum> StmtNumLst;
