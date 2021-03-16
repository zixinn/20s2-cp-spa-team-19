#include "AffectsTEvaluator.h"

using namespace std;

AffectsTEvaluator::AffectsTEvaluator() {

}

bool AffectsTEvaluator::evaluate(unordered_map<string, string> declarations,
    Clause clause, unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    string firstType = getArgType(firstArg, declarations);
    string secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return PKB::affects->getAffectsStarSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) { // known, known
        return PKB::affects->isAffectsStar(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) { // known, s or known, _
        unordered_set<StmtNum> affectedStar = PKB::affects->getAffectsStar(stoi(firstArg));
        if (affectedStar.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) { // known, s
            vector<int> res;
            res.assign(affectedStar.begin(), affectedStar.end());
            tempResults[secondArg] = res;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) { // s, known or _, known
        unordered_set<StmtNum> affectedStar = PKB::affects->getAffectedStar(stoi(secondArg));
        if (affectedStar.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            res.assign(affectedStar.begin(), affectedStar.end());
            tempResults[firstArg] = res;
        }
        return true;

    } else { // s1, s2 or s, _ or _, s
        pair<vector<int>, vector<int>> allAffectsStar = PKB::affects->getAllAffectsStar();
        if (allAffectsStar.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            tempResults[firstArg] = allAffectsStar.first;
            tempResults[secondArg] = allAffectsStar.second;
        } else if (firstType != UNDERSCORE_) { // s, _
            tempResults[firstArg] = allAffectsStar.first;
        } else { // _, s
            tempResults[secondArg] = allAffectsStar.second;
        }
        return true;
    }
}

AffectsTEvaluator::~AffectsTEvaluator() {

}
