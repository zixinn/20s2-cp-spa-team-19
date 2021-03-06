#pragma once

#include <regex>
#include <unordered_set>

#include "../AbstractAPI.h"
#include "../Utility.h"
#include "Clause.h"
#include "Query.h"
#include "QueryUtility.h"

class QueryPreprocessor {
public:
    QueryPreprocessor();

    Query process(string query);

    ~QueryPreprocessor();

private:
    unordered_map<string, string> declarations;
    vector<string> toSelect;
    vector<Clause> clauses;
    bool isSyntacticallyValid = true;
    bool isSemanticallyValid = true;

    unordered_set<string> designEntities;
    unordered_map<string, vector<unordered_set<string>>> validSuchThatArgType;
    unordered_map<string, vector<unordered_set<string>>> validPatternArgType;

    void parseDeclaration(string designEntity, string synonyms);
    bool checkDesignEntity(string designEntity);

    void parseSelect(string select);
    int getNextPos(vector<int> pos);

    void parseToSelect(string synonym);

    void parseSuchThatClauses(string clauses);
    void parseSuchThatClause(string clause);
    void checkSuchThatClause(string rel, vector<string> args);

    void parsePatternClauses(string clauses);
    void parsePatternClause(string clause);
    void checkPatternClause(string syn, vector<string> args);
};
