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
#include "NextBipEvaluator.h"
#include "NextBipTEvaluator.h"
/*#include "AffectsBipEvaluator.h"
#include "AffectsBipTEvaluator.h"*/

// Evaluates queries with the help of helper Evaluator classes
class QueryEvaluator {
public:
    // Constructor for QueryEvaluator
    QueryEvaluator();

    // Evaluates the query and returns a list containing the answers to the query
    list<STRING> evaluate(Query query);

    // Destructor for QueryEvaluator
    ~QueryEvaluator();

private:
    // Maps synonyms declared to its type
    unordered_map<STRING, STRING> declarations;
    // A vector of the synonyms/attrRef to select
    vector<STRING> toSelect;
    // A set of the synonyms to select
    unordered_set<STRING> synonymsToSelect;
    // A vector of the groups of clauses in the query
    vector<vector<Clause>> clauses;
    // Maps synonym declared to possible values for that synonym
    // Each column in the table is represented as an entry in the map
    unordered_map<STRING, vector<int>> results;

    // Evaluates the clause and stores the results for the clause in tempResults
    // Returns false if the clause is false or cannot be satisfied
    bool evaluateClause(Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    // Joins table and results based on common synonyms (if any) and assigns the newResults to results
    // Assumes table is non-empty
    void join(unordered_map<STRING, vector<int>> table, unordered_map<STRING, vector<int>>& results, vector<STRING> commonSynonyms);

    // Evaluates the synonym to select using the results table and returns a list containing the answers
    list<STRING> evaluateSynonymToSelect(vector<STRING> toSelect);
};
