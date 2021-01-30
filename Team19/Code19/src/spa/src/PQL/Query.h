#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"

using namespace std;

class Query {
public:
    Query();
    Query(unordered_map<string, string> declarations, string toSelect, vector<Clause> clauses, bool isValid);

    unordered_map<string, string> getDeclarations();
    string getToSelect();
    vector<Clause> getClauses();
    bool getIsValid();

    ~Query();

private:
    unordered_map<string, string> declarations;
    string toSelect;
    vector<Clause> clauses;
    bool isValid;
};
