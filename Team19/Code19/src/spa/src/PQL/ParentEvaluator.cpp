#include "ParentEvaluator.h"

using namespace std;

ParentEvaluator::ParentEvaluator() {

}

bool ParentEvaluator::evaluate(unordered_map<string, string> declarations, 
    Clause clause, unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    string firstType = getArgType(firstArg, declarations);
    string secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return PKB::parent->getParentSize() > 0;
    }

    if (firstType == INTEGER_ && secondType == INTEGER_) { // known, known
        return PKB::parent->isParent(stoi(firstArg), stoi(secondArg));

    } else if (firstType == INTEGER_ && secondType != INTEGER_) { // known, s or known, _
        unordered_set<StmtNum> children = PKB::parent->getChildren(stoi(firstArg));
        if (children.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) { // known, s
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(children, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
        return true;

    } else if (firstType != INTEGER_ && secondType == INTEGER_) { // s, known or _, known
        int parent = PKB::parent->getParent(stoi(secondArg));
        if (parent == -1) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(vector<int>{ parent }, selectAll(firstType), res);
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
        pair<vector<int>, vector<int>> allParent = PKB::parent->getAllParent();
        if (allParent.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
            pair<vector<int>, vector<int>> res;
            bool nonEmpty = intersectDoubleSynonym(allParent, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res.first;
                tempResults[secondArg] = res.second;
            }
            return nonEmpty;
        } else if (firstType != UNDERSCORE_) { // s, _
            vector<int> allCorrectType = selectAll(firstType);
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allParent.first, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        } else { // _, s
            vector<int> allCorrectType = selectAll(secondType);
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allParent.second, allCorrectType, res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
    }
}

ParentEvaluator::~ParentEvaluator() {

}
