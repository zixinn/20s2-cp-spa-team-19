#include "AffectsBipTEvaluator.h"

using namespace std;

AffectsBipTEvaluator::AffectsBipTEvaluator() {}

bool AffectsBipTEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                                    unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) {  // _, _
        return PKB::affectsBip->getAffectsBipStarSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) {  // known, known
        return PKB::affectsBip->isAffectsBipStar(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ &&
        secondType != INTEGER_) {  // known, s or known, _
        unordered_set<StmtNum> affected = PKB::affectsBip->getAffectsBipStar(stoi(firstArg));
        if (affected.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) {  // known, s
            vector<int> res;
            res.assign(affected.begin(), affected.end());
            tempResults[secondArg] = res;
        }
        return true;

    } else if (firstType != INTEGER_ &&
        secondType == INTEGER_) {  // s, known or _, known
        unordered_set<StmtNum> affected =
            PKB::affectsBip->getAffectedBipStar(stoi(secondArg));
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
        if (firstArg == secondArg) {  // s, s
            vector<int> res;
            vector<StmtNum> allAssign = selectAll(ASSIGN_);
            for (StmtNum s : allAssign) {
                if (PKB::affectsBip->isAffectsBipStar(s, s)) {
                    res.push_back(s);
                }
            }
            if (!res.empty()) {
                tempResults[firstArg] = res;
            }
            return !res.empty();
        }

        pair<vector<StmtNum>, vector<StmtNum>> allAffectsBipStar =
            PKB::affectsBip->getAllAffectsBipStar();
        if (allAffectsBipStar.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) {  // s1, s2
            tempResults[firstArg] = allAffectsBipStar.first;
            tempResults[secondArg] = allAffectsBipStar.second;
        } else if (firstType != UNDERSCORE_) {  // s, _
            tempResults[firstArg] = allAffectsBipStar.first;
        } else {  // _, s
            tempResults[secondArg] = allAffectsBipStar.second;
        }
        return true;
    }
}

AffectsBipTEvaluator::~AffectsBipTEvaluator() {}
