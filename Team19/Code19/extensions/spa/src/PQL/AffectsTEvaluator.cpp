#include "AffectsTEvaluator.h"

using namespace std;

AffectsTEvaluator::AffectsTEvaluator() {

}

bool AffectsTEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                                 unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

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
        if (firstArg == secondArg) { // s, s
            vector<int> res;
            vector<StmtNum> allAssign = selectAll(ASSIGN_);
            for (StmtNum s : allAssign) {
                if (PKB::affects->isAffectsStar(s, s)) {
                    res.push_back(s);
                }
            }
            if (!res.empty()) {
                tempResults[firstArg] = res;
            }
            return !res.empty();
        }

        pair<vector<StmtNum>, vector<StmtNum>> allAffectsStar = PKB::affects->getAllAffectsStar();
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
