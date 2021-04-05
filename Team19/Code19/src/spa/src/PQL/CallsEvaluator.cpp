#include "CallsEvaluator.h"

using namespace std;

CallsEvaluator::CallsEvaluator() {

}

bool CallsEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, 
                              vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == UNDERSCORE_ && secondType == UNDERSCORE_) { // _, _
        vector<StmtNum> callStmts = PKB::stmtTable->getAllCallStmtNums();
        if (callStmts.empty()) {
            return false;
        }
        return true;
    }

    if (firstType == NAME_ && secondType == NAME_) { // known, known
        ID firstProcId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
        ID secondProcId = PKB::procTable->getProcID(trim(secondArg.substr(1, secondArg.size() - 2)));
        return PKB::calls->isCalls(firstProcId, secondProcId);

    } else if (firstType == NAME_ && secondType != NAME_) { // known, s or known, _
        ID firstProcId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
        unordered_set<StmtNum> callees = PKB::calls->getCallees(firstProcId);
        if (callees.empty()) {
            return false;
        }
        if (secondType != UNDERSCORE_) { //secondType == PROCEDURE
            vector<int> res;
            res.assign(callees.begin(), callees.end());
            tempResults[secondArg] = res;
        }
        return true;

    } else if (firstType != NAME_ && secondType == NAME_) { // s, known or _, known
        ID secondProcId = PKB::procTable->getProcID(trim(secondArg.substr(1, secondArg.size() - 2)));
        unordered_set<StmtNum> callers = PKB::calls->getCallers(secondProcId);
        if (callers.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_) { //firstType == PROCEDURE
            vector<int> res;
            res.assign(callers.begin(), callers.end());
            tempResults[firstArg] = res;
        }
        return true;

    } else { // s1, s2 or s, _ or _, s
        if (firstArg == secondArg) {
            return false;
        }
        pair<vector<StmtNum>, vector<StmtNum>> allCalls = PKB::calls->getAllCalls();
        if (allCalls.first.empty()) {
            return false;
        }
        if (firstType != UNDERSCORE_ && secondType != UNDERSCORE_) { // s1, s2
            tempResults[firstArg] = allCalls.first;
            tempResults[secondArg] = allCalls.second;
            return true;
        } else if (firstType != UNDERSCORE_) { // s, _
            tempResults[firstArg] = allCalls.first;
            return true;
        } else { // _, s
            tempResults[secondArg] = allCalls.second;
            return true;
        }
    }
}

CallsEvaluator::~CallsEvaluator() {

}
