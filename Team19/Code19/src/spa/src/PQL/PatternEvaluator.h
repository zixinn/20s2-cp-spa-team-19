#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"
#include "../SP/ParserUtils.h"
#include "../SP/Parser.h"

class PatternEvaluator {
public:
	PatternEvaluator();

	static bool evaluate(unordered_map<string, string> declarations,
		Clause clause, unordered_map<string, vector<int>>& tempResults);

	~PatternEvaluator();

private:
    enum queryTypes {
        varUnderscore = 0,
        variableName = 1,
        varExpr = 2,
        varExprWithUnderscore = 3,
        nameUnderscore = 10,
        nameName = 11,
        nameExpr = 12,
        nameExprWithUnderscore = 13,
        underscoreUnderscore = 20,
        underscoreName = 21,
        underscoreExpr = 22,
        underscoreExprWithUnderscore = 23,
    };

    static int getIndex(vector<string> v, string s);
    static int getQueryType(string firstType, string secondType);
    static string parseExprToExprStr(string input);

    static bool evaluateNameUnderscore(vector<int> stmtNums, string varName, string firstArg,
        unordered_map<string, vector<int>>& tempResults);
    static bool evaluateNameExpression(vector<int> stmtNums, string varName, string firstArg,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
    static bool evaluateNameExpressionWithUnderscore(vector<int> stmtNums, string varName, string firstArg,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
    static bool evaluateVarUnderscore(vector<int> stmtNums, string varName, string firstArg,
        unordered_map<string, vector<int>>& tempResults);
    static bool evaluateVarExpression(vector<int> stmtNums, string varName, string firstArg,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
    static bool evaluateVarExpressionWithUnderscore(vector<int> stmtNums, string varName, string firstArg,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
    static bool evaluateUnderscoreUnderscore(vector<int> stmtNums, string varName,
        unordered_map<string, vector<int>>& tempResults);
    static bool evaluateUnderscoreExpression(vector<int> stmtNums, string varName,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
    static bool evaluateUnderscoreExpressionWithUnderscore(vector<int> stmtNums, string varName,
        string secondArg, unordered_map<string, vector<int>>& tempResults);
};