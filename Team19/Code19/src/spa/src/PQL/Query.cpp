#include "Query.h"

Query::Query() {

}

Query::Query(unordered_map<STRING, STRING> declarations, vector<STRING> toSelect, vector<vector<Clause>> clauses,
             bool isSyntacticallyValid, bool isSemanticallyValid) {
    this->declarations = declarations;
    this->toSelect = toSelect;
    this->clauses = clauses;
    this->isSyntacticallyValid = isSyntacticallyValid;
    this->isSemanticallyValid = isSemanticallyValid;
}

unordered_map<STRING, STRING> Query::getDeclarations() {
    return this->declarations;
}

vector<STRING> Query::getToSelect() {
    return this->toSelect;
}

vector<vector<Clause>> Query::getClauses() {
    return this->clauses;
}

vector<unordered_set<STRING>> Query::getSynonyms() {
    return this->synonyms;
}

bool Query::getIsSyntacticallyValid() {
    return this->isSyntacticallyValid;
}

bool Query::getIsSemanticallyValid() {
    return this->isSemanticallyValid;
}

void Query::setClauses(vector<vector<Clause>> clauses) {
    this->clauses = clauses;
}

void Query::setClausesAtIdx(vector<Clause> clauses, int idx) {
    this->clauses.at(idx) = clauses;
}

void Query::setSynonyms(vector<unordered_set<STRING>> synonyms) {
    this->synonyms = synonyms;
}

bool operator==(const Query& q1, const Query& q2) {
    return q1.declarations == q2.declarations && q1.toSelect == q2.toSelect && q1.clauses == q2.clauses
           && q1.isSyntacticallyValid == q2.isSyntacticallyValid && q1.isSemanticallyValid == q2.isSemanticallyValid;
}

Query::~Query() {

}
