#pragma once

#include "QueryUtility.h"
#include "../SP/ParserUtils.h"
#include "../SP/Parser.h"

// Helper class to evaluate pattern clauses
class PatternEvaluator {
public:
    // Constructor for PatternEvaluator
    PatternEvaluator();

    // Evaluates the clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluate(unordered_map<STRING, STRING> declarations,
                         Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    // Destructor for PatternEvaluator
    ~PatternEvaluator();

private:
    // Enum for the different types of pattern clauses
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

    // Returns the index of string s in the vector v
    static int getIndex(vector<STRING> v, STRING s);
    // Returns the enum representing the type of pattern clause
    static int getQueryType(STRING firstType, STRING secondType);

    // Adds brackets to the expression such that it is left-associative
    static STRING parseExprToExprStr(STRING input);

    // Trims string with quotes
    static STRING trimQuotes(STRING s);
    // Trims string with quotes and underscore
    static STRING trimQuotesUnderscore(STRING s);

    // Evaluates the if pattern clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateIfClause(STRING varName, STRING firstArg, STRING firstType,
                                 unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the while pattern clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateWhileClause(STRING varName, STRING firstArg, STRING firstType,
                                    unordered_map<STRING, vector<int>>& tempResults);

    // Evaluates the assign pattern clause with variable name on the LHS and _ on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateNameUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                       unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with variable name on the LHS and full expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateNameExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                       STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with variable name on the LHS and partial expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateNameExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                     STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with synonym on the LHS and _ on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateVarUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                      unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with synonym on the LHS and full expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateVarExpression(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                      STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with synonym on the LHS and partial expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateVarExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING firstArg,
                                                    STRING secondArg, unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with _ on both the LHS and the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateUnderscoreUnderscore(vector<StmtNum> stmtNums, STRING varName,
                                             unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with _ on the LHS and full expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateUnderscoreExpression(vector<StmtNum> stmtNums, STRING varName, STRING secondArg,
                                             unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the assign pattern clause with _ on the LHS and partial expression on the RHS
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateUnderscoreExpressionWithUnderscore(vector<StmtNum> stmtNums, STRING varName, STRING secondArg,
                                                           unordered_map<STRING, vector<int>>& tempResults);
};
