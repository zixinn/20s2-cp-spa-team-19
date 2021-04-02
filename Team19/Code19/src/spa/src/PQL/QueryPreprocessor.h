#pragma once

#include <regex>
#include <unordered_set>

#include "../AbstractAPI.h"
#include "../Utility.h"
#include "Clause.h"
#include "Query.h"
#include "QueryUtility.h"

// Processes PQL queries into a Query object
class QueryPreprocessor {
public:
    // Constructor for QueryPreprocessor
    QueryPreprocessor();

    // Parses and validates the query and returns a Query object representing the query
    Query process(STRING query);

    // Destructor for QueryPreprocessor
    ~QueryPreprocessor();

private:
    // Maps synonyms declared to its type
    unordered_map<STRING, STRING> declarations;
    // A vector of the synonyms to select
    vector<STRING> toSelect;
    // A vector of the clauses in the query
    vector<Clause> clauses;
    // Boolean to indicate if the query is syntactically valid
    bool isSyntacticallyValid = true;
    // Boolean to indicate if the query is semantically valid
    bool isSemanticallyValid = true;

    // A set of all design entities
    unordered_set<STRING> designEntities;
    // A set of all attrNames
    unordered_set<STRING> attrNames;
    // Maps each design abstraction to a vector of sets,
    // where the nth set includes all valid argument types for the nth argument in the clause
    unordered_map<STRING, vector<unordered_set<STRING>>> validSuchThatArgType;
    // Maps each "assign"/"while"/"if" to a vector of sets,
    // where the nth set includes all valid argument types for the nth argument in the clause
    unordered_map<STRING, vector<unordered_set<STRING>>> validPatternArgType;
    // Maps synonym types to a set of valid attrNames for that type
    unordered_map<STRING, unordered_set<STRING>> attrMap;

    // Parses and validates the declaration for syntactic and semantic errors
    // Adds the declaration to the declarations map if the declaration is valid
    void parseDeclaration(STRING designEntity, STRING synonyms);
    // Returns true if the design entity is valid, false otherwise
    bool checkDesignEntity(STRING designEntity);

    // Parses the validates the select statement
    void parseSelect(STRING select);
    // Returns the minimum non-negative value in the vector
    // Returns -1 if all values in the vector are negative
    int getNextPos(vector<int> pos);

    // Parses and validates the result clause for syntactic and semantic errors
    // Adds each element to the toSelect vector if it is valid
    void parseToSelect(STRING resultCl);
    // Checks that the attrRef (synonym ‘.’ attrName) is syntactically and semantically valid
    void checkAttrRef(STRING synonym, STRING attrName);

    // Parses and splits the relCond into multiple relRef using the "and" keyword
    void parseSuchThatClauses(STRING clauses);
    // Parses and splits each relRef into its relationship and arguments
    void parseSuchThatClause(STRING clause);
    // Validates and checks the relationship and arguments for syntactic and semantic errors
    // Adds the clause to the clauses vector if it is valid
    void checkSuchThatClause(STRING rel, vector<STRING> args);

    // Parses and splits the patternCond into multiple pattern using the "and" keyword
    void parsePatternClauses(STRING clauses);
    // Parses and splits each pattern into its synonym and arguments
    void parsePatternClause(STRING clause);
    // Validates and checks the synonym and arguments for syntactic and semantic errors
    // Adds the clause to the clauses vector if it is valid
    void checkPatternClause(STRING syn, vector<STRING> args);

    // Parses and splits the attrCond into multiple attrCompare using the "and" keyword
    void parseWithClauses(STRING clauses);
    // Parses and splits each attrCompare into its refs
    void parseWithClause(STRING clause);
    // Validates and checks the refs for syntactic and semantic errors
    // Adds the clause to the clauses vector if it is valid
    void checkWithClause(STRING left, STRING right);
    // Checks if the ref in the with clause is valid
    // Returns NAME or INTEGER depending on the type of the ref and empty STRING if the ref is invalid
    STRING checkRef(STRING& ref, unordered_set<STRING>& synonyms, int& numOfKnown, STRING& argType);
};
