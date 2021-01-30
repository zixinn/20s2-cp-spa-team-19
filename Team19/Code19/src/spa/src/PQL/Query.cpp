#include "Query.h"

Query::Query() {

}

Query::Query(unordered_map<string, string> declarations, string toSelect, vector<Clause> suchThatClauses, vector<Clause> patternClauses, bool isValid) {
    this->declarations = declarations;
    this->toSelect = toSelect;
    this->suchThatClauses = suchThatClauses;
    this->patternClauses = patternClauses;
    this->isValid = isValid;
}

unordered_map<string, string> Query::getDeclarations() {
    return this->declarations;
}

string Query::getToSelect() {
    return this->toSelect;
}

vector<Clause> Query::getSuchThatClauses() {
    return this->suchThatClauses;
}

vector<Clause> Query::getPatternClauses() {
    return this->patternClauses;
}

bool Query::getIsValid() {
    return this->isValid;
}

bool operator==(const Query& q1, const Query& q2) {
    return q1.declarations == q2.declarations && q1.toSelect == q2.toSelect
        && q1.suchThatClauses == q2.suchThatClauses && q1.patternClauses == q2.patternClauses
        && q1.isValid == q2.isValid;
}

Query::~Query() {

}
