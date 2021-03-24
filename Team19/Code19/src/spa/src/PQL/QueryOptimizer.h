#pragma once

#include <unordered_set>

#include "../AbstractAPI.h"
#include "Query.h"
#include "QueryUtility.h"

class QueryOptimizer {
public:
    QueryOptimizer();

    void setIsGroup(bool isGroup);
    void setIsOrderClauses(bool isOrderClauses);

    Query optimize(Query query);

    ~QueryOptimizer();

private:
    bool isGroup = true;
    bool isOrderClauses = true;

    void groupClauses(Query& query);
    void dfs(int source, vector<unordered_set<int>> adj, vector<bool>& visited, unordered_set<int>& indexOfSynonymsInGroup);

    void orderClauses(Query& query);
};
