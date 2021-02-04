#pragma once

#include <regex>
#include <unordered_set>

#include "../AbstractAPI.h"
#include "../Utility.h"
#include "Clause.h"
#include "Query.h"

class QueryPreprocessor {
public:
    QueryPreprocessor();

    Query process(string query);

    ~QueryPreprocessor();

private:
    unordered_map<string, string> declarations;
    string toSelect;
    vector<Clause> suchThatClauses;
    vector<Clause> patternClauses;
    bool isValid = true;

    unordered_set<string> designEntities;
    unordered_map<string, vector<unordered_set<string>>> validSuchThatArgType;
    unordered_map<string, vector<unordered_set<string>>> validPatternArgType;

    bool checkSynonymDeclared(string synonym);
    string getArgType(string synonym);

    bool parseDeclaration(string designEntity, string synonyms);
    bool checkDesignEntity(string designEntity);

    bool parseSelect(string select);

    bool parseToSelect(string synonym);

    bool parseSuchThatClause(string clause);
    bool checkSuchThatClause(string rel, vector<string> args);

    bool parsePatternClause(string clause);
    bool checkPatternClause(string syn, vector<string> args);
};
