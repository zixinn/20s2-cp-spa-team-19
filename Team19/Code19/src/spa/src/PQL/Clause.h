#pragma once

#include <unordered_set>
#include <vector>

#include "../Utility.h"

using namespace std;

// Represents a PQL Clause
class Clause {
public:
    Clause();
    Clause(STRING rel, vector<STRING> args, unordered_set<STRING> synonyms, int numOfKnown);

    STRING getRel();
    vector<STRING> getArgs();
    unordered_set<STRING> getSynonyms();
    int getNumOfKnown();
    void replaceSynonym(STRING synonym, STRING replacement);

    friend bool operator==(const Clause& c1, const Clause& c2);
    friend bool operator!=(const Clause& c1, const Clause& c2);

    ~Clause();

private:
    STRING rel;
    vector<STRING> args;
    unordered_set<STRING> synonyms;
    int numOfKnown;

    bool checkReplacement(STRING syn, STRING attrName, STRING synToReplace, STRING replacement);
};
