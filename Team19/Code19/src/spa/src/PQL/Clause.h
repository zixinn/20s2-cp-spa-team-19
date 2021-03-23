#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Clause {
public:
    Clause();
    Clause(string rel, vector<string> args, unordered_set<string> synonyms, int numOfKnown);

    string getRel();
    vector<string> getArgs();
    unordered_set<string> getSynonyms();
    int getNumOfKnown();

    friend bool operator==(const Clause& c1, const Clause& c2);

    ~Clause();

private:
    string rel;
    vector<string> args;
    unordered_set<string> synonyms;
    int numOfKnown;
};
