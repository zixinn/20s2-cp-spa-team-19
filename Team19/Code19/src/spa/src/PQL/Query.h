#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"

using namespace std;

class Query {
public:
    Query();
    Query(unordered_map<string, string> declarations, vector<string> toSelect, vector<vector<Clause>> clauses,
            bool isSyntacticallyValid, bool isSemanticallyValid);

    unordered_map<string, string> getDeclarations();
    vector<string> getToSelect();
    vector<vector<Clause>> getClauses();
    vector<unordered_set<string>> getSynonyms();
    bool getIsSyntacticallyValid();
    bool getIsSemanticallyValid();

    void setClauses(vector<vector<Clause>> clauses);
    void setClausesAtIdx(vector<Clause> clauses, int idx);
    void setSynonyms(vector<unordered_set<string>> synonyms);

    friend bool operator==(const Query& q1, const Query& q2);

    ~Query();

private:
    unordered_map<string, string> declarations;
    vector<string> toSelect;
    vector<vector<Clause>> clauses;
    vector<unordered_set<string>> synonyms;
    bool isSyntacticallyValid;
    bool isSemanticallyValid;
};
