#include "Clause.h"

Clause::Clause() {

}

Clause::Clause(string rel, vector<string> args, unordered_set<string> synonyms) {
    this->rel = rel;
    this->args = args;
    this->synonyms = synonyms;
}

string Clause::getRel() {
    return this->rel;
}

vector<string> Clause::getArgs() {
    return this->args;
}

unordered_set<string> Clause::getSynonyms() {
    return this->synonyms;
}

bool operator==(const Clause& c1, const Clause& c2) {
    return c1.rel == c2.rel && c1.args == c2.args && c1.synonyms == c2.synonyms;
}

Clause::~Clause() {

}
