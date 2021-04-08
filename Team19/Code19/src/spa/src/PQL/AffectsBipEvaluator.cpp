#include "AffectsBipEvaluator.h"

using namespace std;

AffectsBipEvaluator::AffectsBipEvaluator() {}

bool AffectsBipEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                                   unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) {  // _, _
        return PKB::affectsBip->getAffectsBipSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) {  // known, known
        return PKB::affectsBip->isAffectsBip(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) {  // known, s or known, _
        unordered_set<StmtNum> affected = PKB::affectsBip->getAffectsBip(stoi(firstArg));
        if (affected.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) {  // known, s
            vector<int> res;
            res.assign(affected.begin(), affected.end());
            tempResults[secondArg] = res;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) {  // s, known or _, known
        unordered_set<StmtNum> affected = PKB::affectsBip->getAffectedBip(stoi(secondArg));
        if (affected.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            res.assign(affected.begin(), affected.end());
            tempResults[firstArg] = res;
        }
        return true;

    } else {  // s1, s2 or s, _ or _, s
        if (firstArg == secondArg) {  // s, s
            vector<int> res;
            vector<StmtNum> allAssign = selectAll(ASSIGN_);
            for (StmtNum s : allAssign) {
                if (PKB::affectsBip->isAffectsBip(s, s)) {
                    res.push_back(s);
                }
            }
            if (!res.empty()) {
                tempResults[firstArg] = res;
            }
            return !res.empty();
        }

        pair<vector<StmtNum>, vector<StmtNum>> allAffectsBip = PKB::affectsBip->getAllAffectsBip();
        if (allAffectsBip.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) {  // s1, s2
            tempResults[firstArg] = allAffectsBip.first;
            tempResults[secondArg] = allAffectsBip.second;
        } else if (firstType != UNDERSCORE_) {  // s, _
            tempResults[firstArg] = allAffectsBip.first;
        } else {  // _, s
            tempResults[secondArg] = allAffectsBip.second;
        }
        return true;
    }
}

AffectsBipEvaluator::~AffectsBipEvaluator() {}
