#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

class NextEvaluator {
public:
    NextEvaluator();

    static bool evaluate(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults);

    ~NextEvaluator();
};