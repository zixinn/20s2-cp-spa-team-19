#include "Clause.h"

Clause::Clause() {

}

Clause::Clause(string rel, vector<string> args, unordered_set<string> synonyms, int numOfKnown) {
    this->rel = rel;
    this->args = args;
    this->synonyms = synonyms;
    this->numOfKnown = numOfKnown;
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

int Clause::getNumOfKnown() {
    return this->numOfKnown;
}

void Clause::replaceSynonym(string synonym, string replacement) {
    vector<string> newArgs;
    bool replace = false;
    for (string arg : args) {
        int posOfDot = arg.find('.');
        string attrName;
        if (posOfDot != string::npos) { // dot found
            attrName = arg.substr(posOfDot + 1, arg.length());
            arg = arg.substr(0, posOfDot);
            replace = checkReplacement(arg, attrName, synonym, replacement);
        }
        if (arg == synonym || replace) {
            newArgs.push_back(replacement);
            this->synonyms.erase(arg);
        } else {
            newArgs.push_back(arg);
        }
    }
    this->args = newArgs;
    this->numOfKnown++;
}

// to check if synonym.attrName can be replaced 
bool Clause::checkReplacement(string syn, string attrName, string synToReplace, string replacement) {
    if (syn != synToReplace) {
        return false;
    }
    if ((attrName == "varName" && checkName(replacement))
        || (attrName == "procName" && checkName(replacement))
        || (attrName == "stmt#" && checkInteger(replacement))
        || (attrName == "value" && checkInteger(replacement))) {
        return true;
    }
}

bool operator==(const Clause& c1, const Clause& c2) {
    return c1.rel == c2.rel && c1.args == c2.args && c1.synonyms == c2.synonyms && c1.numOfKnown == c2.numOfKnown;
}

Clause::~Clause() {

}
