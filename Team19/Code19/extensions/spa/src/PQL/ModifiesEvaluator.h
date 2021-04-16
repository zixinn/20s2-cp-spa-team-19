#pragma once

#include "QueryUtility.h"

// Helper class to evaluate such that clauses with Modifies relationship
class ModifiesEvaluator {
public:
    // Constructor for ModifiesEvaluator
    ModifiesEvaluator();

    // Evaluates the clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                         unordered_map<STRING, vector<int>>& tempResults);

    // Destructor for ModifiesEvaluator
    ~ModifiesEvaluator();

private:
    // Evaluates the clause with Modifies relationship between statements and variables
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateStmtModifies(unordered_map<STRING, STRING> declarations, Clause clause,
                                     unordered_map<STRING, vector<int>>& tempResults);

    // Evaluates the clause with Modifies relationship between procedures and variables
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateProcModifies(unordered_map<STRING, STRING> declarations, Clause clause,
                                     unordered_map<STRING, vector<int>>& tempResults);
};
