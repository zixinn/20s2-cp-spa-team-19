#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

class ModifiesEvaluator {
public:
    ModifiesEvaluator();

    static bool evaluate(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults);

    ~ModifiesEvaluator();

private:
    static bool evaluateStmtModifies(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults);

    static bool evaluateProcModifies(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults);
};
