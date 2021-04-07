#pragma once

#include "../Utility.h"

using namespace std;

// Represents a PQL Clause
class Clause {
public:
    // Constructor for Clause
    Clause();
    Clause(STRING rel, vector<STRING> args, unordered_set<STRING> synonyms, int numOfKnown);

    // For suchthat-cl, returns the relationship queried
    // For with-cl, returns an empty string
    // For pattern-cl, returns the synonym queried
    STRING getRel();
    // Returns the arguments in the clause
    vector<STRING> getArgs();
    // Returns the synonyms used in the clause
    unordered_set<STRING> getSynonyms();
    // Returns the total number known values in the clause
    int getNumOfKnown();
    // Replaces an argument in the clause with another string
    void replaceSynonym(STRING synonym, STRING replacement);

    // Overriden methods to compare Clause objects
    friend bool operator==(const Clause& c1, const Clause& c2);
    friend bool operator!=(const Clause& c1, const Clause& c2);

    // Destructor for Clause
    ~Clause();

private:
    // For suchthat-cl, refers to relationship queried
    // For with-cl, refers to empty string
    // For pattern-cl, refers to the synonym queried
    STRING rel;
    // Vector of arguments in the clause
    vector<STRING> args;
    // Set of synonyms used in the clause
    unordered_set<STRING> synonyms;
    // Total number of known values in the clause
    int numOfKnown;

    // Returns true if the argument used in the clause can be replaced, false otherwise
    bool checkReplacement(STRING syn, STRING attrName, STRING synToReplace, STRING replacement);
};
