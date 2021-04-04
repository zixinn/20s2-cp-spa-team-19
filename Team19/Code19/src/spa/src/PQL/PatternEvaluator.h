#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"
#include "../SP/ParserUtils.h"
#include "../SP/Parser.h"

// Helper class to evaluate Patterns
class PatternEvaluator {
public:
    PatternEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations,
                         Clause clause, unordered_map<STRING, vector<int>>& tempResults);

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

    static int getIndex(vector<STRING> v, STRING s);
    static int getQueryType(STRING firstType, STRING secondType);
    static STRING parseExprToExprStr(STRING input);

    static bool evaluateIfClause(STRING varName, STRING firstArg, STRING firstType,
                                 unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateWhileClause(STRING varName, STRING firstArg, STRING firstType,
                                    unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateNameUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                       unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateNameExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                       STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateNameExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                     STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateVarUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                      unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateVarExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                      STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateVarExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                    STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateUnderscoreUnderscore(vector<StmtNum> stmtNums, STRING varName,
                                             unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateUnderscoreExpression(vector<StmtNum> stmtNums, STRING varName,
                                             STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    static bool evaluateUnderscoreExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName,
                                                           STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
};