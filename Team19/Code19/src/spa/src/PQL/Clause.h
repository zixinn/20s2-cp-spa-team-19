#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "../Utility.h"

using namespace std;

class Clause {
public:
    Clause();
    Clause(string rel, vector<string> args, unordered_set<string> synonyms, int numOfKnown);

    string getRel();
    vector<string> getArgs();
    unordered_set<string> getSynonyms();
    int getNumOfKnown();
    void replaceSynonym(string synonym, string replacement);

    friend bool operator==(const Clause& c1, const Clause& c2);
    friend bool operator!=(const Clause& c1, const Clause& c2);

    ~Clause();

private:
    string rel;
    vector<string> args;
    unordered_set<string> synonyms;
    int numOfKnown;

    bool checkReplacement(string syn, string attrName, string synToReplace, string replacement);
};
