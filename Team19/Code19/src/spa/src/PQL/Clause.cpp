#include "Clause.h"

Clause::Clause() {

}

Clause::Clause(string rel, vector<string> args) {
    this->rel = rel;
    this->args = args;
}

string Clause::getRel() {
    return this->rel;
}

vector<string> Clause::getArgs() {
    return this->args;
}

bool operator==(const Clause& c1, const Clause& c2) {
    return c1.rel == c2.rel && c1.args == c2.args;
}

Clause::~Clause() {

}
