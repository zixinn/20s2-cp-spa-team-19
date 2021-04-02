#pragma once

#include <STRING>
#include <unordered_map>
#include <vector>

#include "Clause.h"

using namespace std;

// Represents a PQL query
class Query {
public:
    Query();
    Query(unordered_map<STRING, STRING> declarations, vector<STRING> toSelect, vector<vector<Clause>> clauses,
          bool isSyntacticallyValid, bool isSemanticallyValid);

    unordered_map<STRING, STRING> getDeclarations();
    vector<STRING> getToSelect();
    vector<vector<Clause>> getClauses();
    vector<unordered_set<STRING>> getSynonyms();
    bool getIsSyntacticallyValid();
    bool getIsSemanticallyValid();

    void setClauses(vector<vector<Clause>> clauses);
    void setClausesAtIdx(vector<Clause> clauses, int idx);
    void setSynonyms(vector<unordered_set<STRING>> synonyms);

    friend bool operator==(const Query& q1, const Query& q2);

    ~Query();

private:
    unordered_map<STRING, STRING> declarations;
    vector<STRING> toSelect;
    vector<vector<Clause>> clauses;
    vector<unordered_set<STRING>> synonyms;
    bool isSyntacticallyValid;
    bool isSemanticallyValid;
};
