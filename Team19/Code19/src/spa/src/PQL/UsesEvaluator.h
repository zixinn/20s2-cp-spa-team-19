#pragma once

#include "QueryUtility.h"

// Helper class to evaluate such that clauses with Uses relationship
class UsesEvaluator {
public:
    // Constructor for UsesEvaluator
    UsesEvaluator();

    // Evaluates the clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                         unordered_map<STRING, vector<int>>& tempResults);

    // Destructor for UsesEvaluator
    ~UsesEvaluator();
private:
    // Evaluates the clause with Uses relationship between statements and variables
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateStmtUses(unordered_map<STRING, STRING> declarations, Clause clause,
                                 unordered_map<STRING, vector<int>>& tempResults);

    // Evaluates the clause with Uses relationship between procedures and variables
    // Stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluateProcUses(unordered_map<STRING, STRING> declarations, Clause clause,
                                 unordered_map<STRING, vector<int>>& tempResults);
};
