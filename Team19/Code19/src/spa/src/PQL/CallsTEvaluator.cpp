#include "CallsEvaluator.h"

/*using namespace std;

CallsEvaluator::CallsEvaluator() {

}

bool CallsEvaluator::evaluate(unordered_map<string, string> declarations, Clause clause, unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    string firstType = getArgType(firstArg, declarations);
    string secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        return true; // part of SIMPLE rules, will be checked by SP [program: procedure+]
    }

    if (firstType == NAME_ && secondType == NAME_) { // known, known
        int firstProcId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
        int secondProcId = PKB::procTable->getProcID(trim(secondArg.substr(1, secondArg.size() - 2)));
        return PKB::Calls->isCallsStar(firstProcId, secondProcId);
    }
    else if (firstType == NAME_ && secondType != NAME_) { // known, s or known, _
        int firstProcId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
        unordered_set<StmtNum> callees = PKB::Calls->getCalleesStar(firstProcId);
        if (callees.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(callees, selectAll(secondType), res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
        return true;
    }
    else if (firstType != NAME_ && secondType == NAME_) { // s, known or _, known
        int secondProcId = PKB::procTable->getProcID(trim(secondArg.substr(1, secondArg.size() - 2)));
        unordered_set<StmtNum> callers = PKB::calls->getCallersStar(secondProcId);
        if (callers.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) {
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(callers, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        }
        return true;

    }
    else { // s1, s2 or s, _ or _, s
        if (firstArg == secondArg) {
            return false;
        }
        pair<vector<int>, vector<int>> allCalls = PKB::calls->getAllCallsStar();
        if (allCalls.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
            pair<vector<int>, vector<int>> res;
            bool nonEmpty = intersectDoubleSynonym(allCalls, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res.first;
                tempResults[secondArg] = res.second;
            }
            return nonEmpty;
        }
        else if (firstType != UNDERSCORE_) { // s, _
            vector<int> allCorrectType = selectAll(firstType);
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allCalls.first, allCorrectType, res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;
        }
        else { // _, s
            vector<int> allCorrectType = selectAll(secondType);
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(allCalls.second, allCorrectType, res);
            if (nonEmpty) {
                tempResults[secondArg] = res;
            }
            return nonEmpty;
        }
    }
}

CallsEvaluator::~CallsEvaluator() {

}
*/