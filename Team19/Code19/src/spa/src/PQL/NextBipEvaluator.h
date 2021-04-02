#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

// Helper class to evaluate NextBip
class NextBipEvaluator {
public:
    NextBipEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    ~NextBipEvaluator();
};
