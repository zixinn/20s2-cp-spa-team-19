#include "Query.h"

Query::Query() {

}

Query::Query(unordered_map<string, string> declarations, string toSelect, vector<Clause> clauses, bool isValid) {
    this->declarations = declarations;
    this->toSelect = toSelect;
    this->clauses = clauses;
    this->isValid = isValid;
}

unordered_map<string, string> Query::getDeclarations() {
    return this->declarations;
}

string Query::getToSelect() {
    return this->toSelect;
}

vector<Clause> Query::getClauses() {
    return this->clauses;
}

bool Query::getIsValid() {
    return this->isValid;
}

bool operator==(const Query& q1, const Query& q2) {
    return q1.declarations == q2.declarations && q1.toSelect == q2.toSelect
        && q1.clauses == q2.clauses && q1.isValid == q2.isValid;
}

Query::~Query() {

}
