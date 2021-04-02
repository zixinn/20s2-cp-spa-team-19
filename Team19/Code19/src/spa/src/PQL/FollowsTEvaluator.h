#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

class FollowsTEvaluator {
public:
    FollowsTEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING,
                         vector<StmtNum>>& tempResults);

    ~FollowsTEvaluator();
};
