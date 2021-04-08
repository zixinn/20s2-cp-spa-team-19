#pragma once

#include "Clause.h"

using namespace std;

// Represents a PQL query
class Query {
public:
    // Constructor for Query
    Query();
    Query(unordered_map<STRING, STRING> declarations, vector<STRING> toSelect, vector<vector<Clause>> clauses,
          bool isSyntacticallyValid, bool isSemanticallyValid);

    // Returns a map of the synonyms declared to its synonym types
    unordered_map<STRING, STRING> getDeclarations();
    // Returns a vector of the arguments in a Select clause
    vector<STRING> getToSelect();
    // Returns a vector of vectors of clauses
    // where each vector of clauses is a group of clauses
    vector<vector<Clause>> getClauses();
    // Returns a vector of sets of synonyms
    // where each set of synonyms correspond to the synonyms found in a group of clauses
    vector<unordered_set<STRING>> getSynonyms();
    // Returns true if the query is syntactically valid, false otherwise
    bool getIsSyntacticallyValid();
    // Returns true if the query is semantically valid, false otherwise
    bool getIsSemanticallyValid();

    // Sets a new vector of vectors of clauses in the Query Object
    void setClauses(vector<vector<Clause>> clauses);
    // Sets a new vector of clauses at a specific index in the vector of vectors clauses
    void setClausesAtIdx(vector<Clause> clauses, int idx);
    // Sets a new vector of sets of synonyms
    void setSynonyms(vector<unordered_set<STRING>> synonyms);

    // Overrides the equals method to compare Query objects
    friend bool operator==(const Query& q1, const Query& q2);

    // Destructor for Query
    ~Query();

private:
    // Maps synonyms declared to its type
    unordered_map<STRING, STRING> declarations;
    // A vector of the synonyms to select
    vector<STRING> toSelect;
    // A vector of vectors of clauses
    // where each vector of clauses is a group of clauses
    vector<vector<Clause>> clauses;
    // A vector of sets of synonyms
    // where each set of synonyms correspond to the synonyms found in a group of clauses
    vector<unordered_set<STRING>> synonyms;
    // Boolean to indicate if the query is syntactically valid
    bool isSyntacticallyValid;
    // Boolean to indicate if the query is semantically valid
    bool isSemanticallyValid;
};
