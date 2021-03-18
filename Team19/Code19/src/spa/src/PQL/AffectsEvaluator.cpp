#include "AffectsEvaluator.h"

using namespace std;

AffectsEvaluator::AffectsEvaluator() {

}

bool AffectsEvaluator::evaluate(unordered_map<string, string> declarations,
    Clause clause, unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    string firstType = getArgType(firstArg, declarations);
    string secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return PKB::affects->getAffectsSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) { // known, known
        return PKB::affects->isAffects(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) { // known, s or known, _
        unordered_set<StmtNum> affected = PKB::affects->getAffects(stoi(firstArg));
        if (affected.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) { // known, s
            vector<int> res;
            res.assign(affected.begin(), affected.end());
            tempResults[secondArg] = res;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) { // s, known or _, known
        unordered_set<StmtNum> affected = PKB::affects->getAffected(stoi(secondArg));
        if (affected.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            res.assign(affected.begin(), affected.end());
            tempResults[firstArg] = res;
        }
        return true;

    } else { // s1, s2 or s, _ or _, s
        pair<vector<int>, vector<int>> allAffects = PKB::affects->getAllAffects();
        if (allAffects.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            tempResults[firstArg] = allAffects.first;
            tempResults[secondArg] = allAffects.second;
        } else if (firstType != UNDERSCORE_) { // s, _
            tempResults[firstArg] = allAffects.first;
        } else { // _, s
            tempResults[secondArg] = allAffects.second;
        }
        return true;
    }
}

AffectsEvaluator::~AffectsEvaluator() {

}