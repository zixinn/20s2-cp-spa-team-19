#include "Query.h"

Query::Query() {

}

Query::Query(unordered_map<string, string> declarations, vector<string> toSelect, vector<vector<Clause>> clauses,
             bool isSyntacticallyValid, bool isSemanticallyValid) {
    this->declarations = declarations;
    this->toSelect = toSelect;
    this->clauses = clauses;
    this->isSyntacticallyValid = isSyntacticallyValid;
    this->isSemanticallyValid = isSemanticallyValid;
}

unordered_map<string, string> Query::getDeclarations() {
    return this->declarations;
}

vector<string> Query::getToSelect() {
    return this->toSelect;
}

vector<vector<Clause>> Query::getClauses() {
    return this->clauses;
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

bool operator==(const Query& q1, const Query& q2) {
    return q1.declarations == q2.declarations && q1.toSelect == q2.toSelect && q1.clauses == q2.clauses
            && q1.isSyntacticallyValid == q2.isSyntacticallyValid && q1.isSemanticallyValid == q2.isSemanticallyValid;
}

Query::~Query() {

}
