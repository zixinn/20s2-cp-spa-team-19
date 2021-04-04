#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

// Helper class to evaluate statment and procedural Uses
class UsesEvaluator {
public:
    UsesEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations,
                         Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    ~UsesEvaluator();
private:
    static bool evaluateStmtUses(unordered_map<STRING, STRING> declarations,
                                 Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    static bool evaluateProcUses(unordered_map<STRING, STRING> declarations,
                                 Clause clause, unordered_map<STRING, vector<int>>& tempResults);
};