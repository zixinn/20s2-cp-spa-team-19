#include "FollowsTEvaluator.h"

using namespace std;

FollowsTEvaluator::FollowsTEvaluator() {

}

bool FollowsTEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING,
                                 vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return PKB::follows->getFollowsStarSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) { // known, known
        return PKB::follows->isFollowsStar(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) { // known, s or known, _
        unordered_set<StmtNum> followers = PKB::follows->getFollowerStar(stoi(firstArg));
        if (followers.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(followers, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) { // s, known or _, known
        unordered_set<StmtNum> followees = PKB::follows->getFolloweeStar(stoi(secondArg));
        if (followees.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(followees, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        }
        return true;

    } else { // s1, s2 or s, _ or _, s
        if (firstArg == secondArg) {
            return false;
        }
        pair<vector<StmtNum>, vector<StmtNum>> allFollowsStar = PKB::follows->getAllFollowsStar();
        if (allFollowsStar.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
            pair<vector<int>, vector<int>> res;
            bool nonEmpty = intersectDoubleSynonym(allFollowsStar, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res.first;
                tempResults[secondArg] = res.second;
            }
            return nonEmpty;
        } else if (firstType != UNDERSCORE_) { // s, _
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allFollowsStar.first, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        } else { // _, s
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allFollowsStar.second, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
    }
}

FollowsTEvaluator::~FollowsTEvaluator() {

}
