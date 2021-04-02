#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

// Helper function to evaluate Affects
class CallsEvaluator {
public:
    CallsEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<StmtNum>>& tempResults);

    ~CallsEvaluator();
};
