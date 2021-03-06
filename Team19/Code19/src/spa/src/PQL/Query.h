#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"

using namespace std;

class Query {
public:
    Query();
    Query(unordered_map<string, string> declarations, string toSelect, vector<Clause> clauses,
            bool isSyntacticallyValid, bool isSemanticallyValid);

    unordered_map<string, string> getDeclarations();
    string getToSelect();
    vector<Clause> getClauses();
    bool getIsSyntacticallyValid();
    bool getIsSemanticallyValid();

    friend bool operator==(const Query& q1, const Query& q2);

    ~Query();

private:
    unordered_map<string, string> declarations;
    string toSelect;
    vector<Clause> clauses;
    bool isSyntacticallyValid;
    bool isSemanticallyValid;
};
