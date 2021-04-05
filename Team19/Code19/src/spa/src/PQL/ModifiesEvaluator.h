#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

// Helper class to evaluate statement and procedural Modifies
class ModifiesEvaluator {
public:
    ModifiesEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    ~ModifiesEvaluator();

private:
    static bool evaluateStmtModifies(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    static bool evaluateProcModifies(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults);
};
