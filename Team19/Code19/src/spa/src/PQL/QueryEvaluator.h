#pragma once

#include <list>
#include <unordered_set>

#include "../AbstractAPI.h"
#include "../PKB/PKB.h"
#include "Clause.h"
#include "Query.h"

class QueryEvaluator {
public:
    QueryEvaluator();

    list<string> evaluate(Query query);

    ~QueryEvaluator();

private:
    unordered_map<string, string> declarations;
    string toSelect;
    vector<Clause> suchThatClauses;
    vector<Clause> patternClauses;
    unordered_map<string, vector<int>> results; // maps synonym declared to possible values for that synonym,
                                                // each column in the table is represented as an entry in the map

    vector<int> selectAll(string synonymType);

    bool evaluateClause(Clause clause, unordered_map<string, vector<int>>& tempResults);

    void join(unordered_map<string, vector<int>> table);

    list<string> evaluateSynonymToSelect(string toSelect);
};
