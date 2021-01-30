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


Query::~Query() {

}
