#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "../../autotester/src/AbstractWrapper.h"

#define PROCEDURE "procedure"
#define STMTLST "stmtLst"
#define STMT "stmt"
#define READ "read"
#define PRINT "print"
#define ASSIGN "assign"
#define CALL "call"
#define WHILE "while"
#define IF "if"
#define VARIABLE "variable"
#define CONSTANT "constant"
#define INTEGER "integer"
#define UNDERSCORE "underscore"
#define NAME "name"
#define EXPRESSION "expression"
#define EXPRESSIONWITHUNDERSCORE "expressionwithunderscore"

using namespace std;

typedef int StmtNum;
typedef int ID;
typedef int CONST;
typedef std::string STRING;
typedef std::vector<StmtNum> StmtNumLst;
