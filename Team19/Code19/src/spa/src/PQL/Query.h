#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"

using namespace std;

class Query {
public:
    Query();
    Query(unordered_map<string, string> declarations, string toSelect, vector<Clause> suchThatClauses, vector<Clause> patternClauses, bool isValid);

    unordered_map<string, string> getDeclarations();
    string getToSelect();
    vector<Clause> getSuchThatClauses();
    vector<Clause> getPatternClauses();
    bool getIsValid();

    friend bool operator==(const Query& q1, const Query& q2);

    ~Query();

private:
    unordered_map<string, string> declarations;
    string toSelect;
    vector<Clause> suchThatClauses;
    vector<Clause> patternClauses;
    bool isValid;
};
