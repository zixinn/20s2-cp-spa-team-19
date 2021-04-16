#include "NextTEvaluator.h"

using namespace std;

NextTEvaluator::NextTEvaluator() {

}

bool NextTEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                              unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return PKB::next->getNextStarSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) { // known, known
        return PKB::next->isNextStar(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) { // known, s or known, _
        unordered_set<ProgLine> next = PKB::next->getNextStar(stoi(firstArg));
        if (next.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(next, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) { // s, known or _, known
        unordered_set<ProgLine> previous = PKB::next->getPreviousStar(stoi(secondArg));
        if (previous.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(previous, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        }
        return true;

    } else { // s1, s2 or s, _ or _, s
        if (firstArg == secondArg) { // s, s
            vector<int> res;
            vector<ProgLine> allLines = selectAll(firstType);
            for (ProgLine n : allLines) {
                if (PKB::next->isNextStar(n, n)) {
                    res.push_back(n);
                }
            }
            if (!res.empty()) {
                tempResults[firstArg] = res;
            }
            return !res.empty();
        }

        pair<vector<ProgLine>, vector<ProgLine>> allNext = PKB::next->getAllNextStar();
        if (allNext.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
            pair<vector<int>, vector<int>> res;
            bool nonEmpty = intersectDoubleSynonym(allNext, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res.first;
                tempResults[secondArg] = res.second;
            }
            return nonEmpty;
        } else if (firstType != UNDERSCORE_) { // s, _
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allNext.first, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        } else { // _, s
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allNext.second, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
    }
}

NextTEvaluator::~NextTEvaluator() {

}
