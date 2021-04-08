#include "Clause.h"

Clause::Clause() {

}

Clause::Clause(STRING rel, vector<STRING> args, unordered_set<STRING> synonyms, int numOfKnown) {
    this->rel = rel;
    this->args = args;
    this->synonyms = synonyms;
    this->numOfKnown = numOfKnown;
}

STRING Clause::getRel() {
    return this->rel;
}

vector<STRING> Clause::getArgs() {
    return this->args;
}

unordered_set<STRING> Clause::getSynonyms() {
    return this->synonyms;
}

int Clause::getNumOfKnown() {
    return this->numOfKnown;
}

void Clause::replaceSynonym(STRING synonym, STRING replacement) {
    vector<STRING> newArgs;
    for (STRING arg : args) {
        bool replace = false;
        STRING syn = arg;
        int posOfDot = syn.find('.');
        STRING attrName;
        if (posOfDot != STRING::npos) { // dot found
            attrName = syn.substr(posOfDot + 1, syn.length());
            syn = syn.substr(0, posOfDot);
            replace = checkReplacement(syn, attrName, synonym, replacement);
        }
        if (arg == synonym || replace) {
            newArgs.push_back(replacement);
            this->synonyms.erase(syn);
            this->numOfKnown++;
        } else {
            newArgs.push_back(arg);
        }
    }
    this->args = newArgs;
}

// Checks if synonym.attrName can be replaced
bool Clause::checkReplacement(STRING syn, STRING attrName, STRING synToReplace, STRING replacement) {
    if (syn != synToReplace) {
        return false;
    }
    if ((attrName == "varName" && !checkInteger(replacement))
            || (attrName == "procName" && !checkInteger(replacement))
            || (attrName == "stmt#" && checkInteger(replacement))
            || (attrName == "value" && checkInteger(replacement))) {
        return true;
    }
    return false;
}

bool operator==(const Clause& c1, const Clause& c2) {
    return c1.rel == c2.rel && c1.args == c2.args && c1.synonyms == c2.synonyms && c1.numOfKnown == c2.numOfKnown;
}

bool operator!=(const Clause& c1, const Clause& c2) {
    return !(c1 == c2);
}

Clause::~Clause() {

}
