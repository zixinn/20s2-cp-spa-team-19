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

Clause::~Clause() {

}
