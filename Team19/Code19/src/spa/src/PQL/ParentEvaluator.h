#pragma once

#include "QueryUtility.h"

// Helper class to evaluate such that clauses with Parent relationship
class ParentEvaluator {
public:
    // Constructor for ParentEvaluator
    ParentEvaluator();

    // Evaluates the clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                         unordered_map<STRING, vector<int>>& tempResults);

    // Destructor for ParentEvaluator
    ~ParentEvaluator();
};
