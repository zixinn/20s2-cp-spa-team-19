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
    bool getIsSyntacticallyValid();
    bool getIsSemanticallyValid();

    void setClauses(vector<vector<Clause>> clauses);

    friend bool operator==(const Query& q1, const Query& q2);

    ~Query();

private:
    unordered_map<string, string> declarations;
    vector<string> toSelect;
    vector<vector<Clause>> clauses;
    bool isSyntacticallyValid;
    bool isSemanticallyValid;
};
