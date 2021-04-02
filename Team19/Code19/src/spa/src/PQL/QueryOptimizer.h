#pragma once

#include <unordered_set>

#include "../AbstractAPI.h"
#include "Query.h"
#include "QueryUtility.h"

// Optimises queries by grouping and sorting queries
class QueryOptimizer {
public:
    QueryOptimizer();

    void setIsGroup(bool isGroup);
    void setIsOrderClauses(bool isOrderClauses);
    void setIsOrderGroups(bool isOrderGroups);
    void setIsRewriteClauses(bool isRewriteClauses);

    Query optimize(Query query);

    ~QueryOptimizer();

private:
    bool isGroup = true;
    bool isOrderClauses = true;
    bool isOrderGroups = true;
    bool isRewriteClauses = true;

    void groupClauses(Query& query);
    void orderGroups(Query& query);
    void dfs(int source, vector<unordered_set<int>> adj, vector<bool>& visited, unordered_set<int>& indexOfSynonymsInGroup);
    void orderClauses(Query& query);
    void rewriteClauses(Query& query);
};
