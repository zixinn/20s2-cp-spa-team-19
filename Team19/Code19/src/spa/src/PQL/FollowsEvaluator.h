#pragma once

#include "Clause.h"
#include "QueryUtility.h"

class FollowsEvaluator {
public:
    FollowsEvaluator();

    static bool evaluate(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults);

    ~FollowsEvaluator();
};
