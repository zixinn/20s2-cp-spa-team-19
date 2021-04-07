#pragma once

#include "Query.h"
#include "QueryUtility.h"

// Optimises queries by grouping and sorting clauses in the query
class QueryOptimizer {
public:
    // Constructor for QueryOptimizer
    QueryOptimizer();

    // Sets a flag in the query optimizer to group clauses
    void setIsGroup(bool isGroup);
    // Sets a flag in the query optimizer to order clauses in groups
    void setIsOrderClauses(bool isOrderClauses);
    // Sets a flag in the query optimizer to order groups
    void setIsOrderGroups(bool isOrderGroups);
    // Sets a flag in the query optimizer to rewrite certain clauses
    // by replacing synonyms with their known value
    void setIsRewriteClauses(bool isRewriteClauses);

    // Optimizes the clauses by grouping them, ordering them in their groups,
    // ordering the groups and rewriting clauses
    Query optimize(Query query);

    // Destructor for QueryOptimizer
    ~QueryOptimizer();

private:
    // Flags set to true by default for query optimizer to group clauses,
    // order clauses, order groups and rewrite clauses
    bool isGroup = true;
    bool isOrderClauses = true;
    bool isOrderGroups = true;
    bool isRewriteClauses = true;

    // Groups clauses by clauses without synonyms and clauses with connected synonyms
    void groupClauses(Query& query);
    // Orders groups by placing groups without synonyms in front,
    // followed by groups that do not return results in Select
    void orderGroups(Query& query);
    // Performs a depth-first search to get indexes of synonyms
    void dfs(int source, vector<unordered_set<int>> adj, vector<bool>& visited,
             unordered_set<int>& indexOfSynonymsInGroup);
    // Orders clauses by the number of synonyms already computed,
    // then by the number of known arguments in the clauses,
    // then by the size of the relevant tables in PKB
    void orderClauses(Query& query);
    // Rewrites clauses by checking for the existence of with-clauses with one synonym and one known value
    // and replacing all occurrences of that synonym in other clauses with the known value
    void rewriteClauses(Query& query);
};
