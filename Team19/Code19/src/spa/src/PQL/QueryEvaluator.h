#pragma once

#include <list>
#include <unordered_set>

#include "../AbstractAPI.h"
#include "../PKB/PKB.h"
#include "Clause.h"
#include "Query.h"
#include "QueryUtility.h"
#include "FollowsEvaluator.h"
#include "FollowsTEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentTEvaluator.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsTEvaluator.h"
#include "NextEvaluator.h"
#include "NextTEvaluator.h"
#include "AffectsEvaluator.h"
#include "AffectsTEvaluator.h"
#include "PatternEvaluator.h"
#include "WithEvaluator.h"

class QueryEvaluator {
public:
    // Constructor for QueryEvaluator
    QueryEvaluator();

    // Evaluates the query and returns a list containing the answers to the query
    list<string> evaluate(Query query);

    // Destructor for QueryEvaluator
    ~QueryEvaluator();

private:
    // Maps synonyms declared to its type
    unordered_map<string, string> declarations;
    // A vector of the synonyms/attrRef to select
    vector<string> toSelect;
    // A set of the synonyms to select
    unordered_set<string> synonymsToSelect;
    // A vector of the groups of clauses in the query
    vector<vector<Clause>> clauses;
    // Maps synonym declared to possible values for that synonym
    // Each column in the table is represented as an entry in the map
    unordered_map<string, vector<int>> results;

    // Evaluates the clause and stores the results for the clause in tempResults
    // Returns false if the clause is false or cannot be satisfied
    bool evaluateClause(Clause clause, unordered_map<string, vector<int>>& tempResults);

    // Joins table and results based on common synonyms (if any) and assigns the newResults to results
    void join(unordered_map<string, vector<int>> table, unordered_map<string, vector<int>>& results);

    // Evaluates the synonym to select using the results table and returns a list containing the answers
    list<string> evaluateSynonymToSelect(vector<string> toSelect);
};
