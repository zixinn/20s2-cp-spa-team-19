#pragma once

#include <unordered_set>

#include "../AbstractAPI.h"
#include "Query.h"

class QueryOptimizer {
public:
    QueryOptimizer();

    void setIsGroup(bool b);

    Query optimize(Query query);

    ~QueryOptimizer();

private:
    bool isGroup = true;

    void groupClauses(Query& query);
    void dfs(int source, vector<unordered_set<int>> adj, vector<bool>& visited, unordered_set<int>& indexOfSynonymsInGroup);
};
