#include "Calls.h"
#include "PKB.h"

Calls::Calls() {
    isCyclic = false;
}

bool Calls::hasCyclicalCall() {
    return isCyclic;
}

bool Calls::isCalls(ProcID p, ProcID q) {
    unordered_map<ProcID, unordered_set<ProcID>>::const_iterator result = callsMap.find(p);
    return result != callsMap.end() && result->second.find(q) != result->second.end();
}

bool Calls::isCallsStar(ProcID p, ProcID q) {
    unordered_map<ProcID, unordered_set<ProcID>>::const_iterator result = callsStarMap.find(p);
    return result != callsStarMap.end() && result->second.find(q) != result->second.end();
}

ProcID Calls::getCalleeInStmt(StmtNum stmtNum) {
    if (callsStmtCalleeMap.find(stmtNum) == callsStmtCalleeMap.end()) {
        return -1;
    }
    return callsStmtCalleeMap.find(stmtNum)->second;
}

unordered_set<StmtNum> const &Calls::getStmtNumThatCallsCallee(ProcID calleeID) const {
    if (reverseCallsStmtCalleeMap.find(calleeID) == reverseCallsStmtCalleeMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseCallsStmtCalleeMap.find(calleeID)->second;
}

const unordered_set<ProcID> & Calls::getCallers(ProcID q) const {
    if (reverseCallsMap.find(q) == reverseCallsMap.end()) {
        static unordered_set<ProcID> empty = unordered_set<ProcID>({});
        return empty;
    }
    return reverseCallsMap.find(q)->second;
}

const unordered_set<ProcID> & Calls::getCallees(ProcID p) const {
    if (callsMap.find(p) == callsMap.end()) {
        static unordered_set<ProcID> empty = unordered_set<ProcID>({});
        return empty;
    }
    return callsMap.find(p)->second;
}

const unordered_set<ProcID> & Calls::getCallersStar(ProcID q) const {
    if (reverseCallsStarMap.find(q) == reverseCallsStarMap.end()) {
        static unordered_set<ProcID> empty = unordered_set<ProcID>({});
        return empty;
    }
    return reverseCallsStarMap.find(q)->second;
}

const unordered_set<ProcID> & Calls::getCalleesStar(ProcID p) const {
    if (callsStarMap.find(p) == callsStarMap.end()) {
        static unordered_set<ProcID> empty = unordered_set<ProcID>({});
        return empty;
    }
    return callsStarMap.find(p)->second;
}

unordered_set<StmtNum> const &Calls::getStmtsOfCalls(ProcID p, ProcID q) {
    if (!isCalls(p, q)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return callsStmtMap.find(p)->second.find(q)->second;
}

pair<vector<ProcID>, vector<ProcID>> Calls::getAllCalls() {
    vector<ProcID> ps, qs;
    for (auto &it : callsMap) {
        for (ProcID q : it.second) {
            ps.push_back(it.first);
            qs.push_back(q);
        }
    }
    return make_pair(ps, qs);
}

pair<vector<ProcID>, vector<ProcID>> Calls::getAllCallsStar() {
    vector<ProcID> ps, qs;
    for (auto &it : callsStarMap) {
        for (ProcID q : it.second) {
            ps.push_back(it.first);
            qs.push_back(q);
        }
    }
    return make_pair(ps, qs);
}

int Calls::getCallsSize() {
    int cnt = 0;
    for (auto &it : callsMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Calls::getCallsStarSize() {
    int cnt = 0;
    for (auto &it : callsStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Calls::storeCalls(StmtNum stmtNum, ProcID p, ProcID q) {
    if (callsRawInfoTable.find(stmtNum) != callsRawInfoTable.end()) {
        return false;
    }
    callsRawInfoTable.insert({stmtNum, make_pair(p, q)});
    return true;
}

bool Calls::hasCalls(StmtNum stmtNum) {
    return callsRawInfoTable.find(stmtNum) != callsRawInfoTable.end();
}

bool Calls::processCalls() {
    ProcID p, q;
    StmtNum stmtNum;
    for (auto &it : callsRawInfoTable) {
        stmtNum = it.first;
        p = it.second.first;
        q = it.second.second;

        auto it2 = callsMap.find(p);
        if (it2 == callsMap.end()) {
            callsMap.insert({p, unordered_set<ProcID>({q})});
        } else {
            it2->second.insert(q);
        }

        it2 = reverseCallsMap.find(q);
        if (it2 == reverseCallsMap.end()) {
            reverseCallsMap.insert({q, unordered_set<ProcID>({p})});
        } else {
            it2->second.insert(p);
        }

        if (callsStmtMap.find(p) == callsStmtMap.end()) {
            callsStmtMap.insert({p, {}});
        }

        if (callsStmtMap.find(p)->second.find(q) == callsStmtMap.find(p)->second.end()) {
            callsStmtMap.find(p)->second.insert({q, unordered_set<StmtNum>({stmtNum})});
        } else {
            callsStmtMap.find(p)->second.find(q)->second.insert(stmtNum);
        }

        callsStmtCalleeMap.insert({stmtNum, q});

        auto it3 = reverseCallsStmtCalleeMap.find(q);
        if (it3 == reverseCallsStmtCalleeMap.end()) {
            reverseCallsStmtCalleeMap.insert({q, unordered_set<StmtNum>({stmtNum})});
        } else {
            it3->second.insert(stmtNum);
        }
    }

    populateCallsStar();

    if (!isCyclic) {
        updateAllUsesAndModifies();
        return true;
    }
    return false;
}

bool Calls::storeCallStar(ProcID p, ProcID q) {
    if (isCallsStar(p, q)) {
        return false;
    }

    auto it = callsStarMap.find(p);
    if (it == callsStarMap.end()) {
        callsStarMap.insert({p, unordered_set<StmtNum>({q})});
    } else {
        it->second.insert(q);
    }

    it = reverseCallsStarMap.find(q);
    if (it == reverseCallsStarMap.end()) {
        reverseCallsStarMap.insert({q, unordered_set<StmtNum>({p})});
    } else {
        it->second.insert(p);
    }
    return true;
}

void Calls::populateCallsStar() {
    unordered_set<ProcID> qs;
    ProcID curr;
    for (auto &it : callsMap) {
        curr = it.first;
        list<int> queue;
        queue.push_back(curr);
        while (!queue.empty()) {
            qs = getCallees(queue.front());
            queue.pop_front();
            for (ProcID q : qs) {
                if (curr == q) {
                    isCyclic = true;
                    break;
                } else {
                    storeCallStar(curr, q);
                    queue.push_back(q);
                }
            }
        }
    }

}

void Calls::updateAllUsesAndModifies() {
    // For each callers p that we have, find out who their direct callees q's are
    // For each callee procedure q, process q first (DFS with implicit stack)
    // Find the stmtNums where the Calls(p, q) occurs
    // For each stmtNum, storeUsesModifies(stmtNum, p, q)

    for (ProcID p : getAllCalls().first) {
        updateUsesAndModifiesForProcedure(p);
    }
}

void Calls::updateUsesAndModifiesForProcedure(ProcID p) {
    processedProcedures.insert(p);
    for (ProcID q : getCallees(p)) {
        if (processedProcedures.find(q) == processedProcedures.end()) {
            processedProcedures.insert(q);
            updateUsesAndModifiesForProcedure(q);
            for (StmtNum stmtNum : getStmtsOfCalls(p, q)) {
                storeUsesAndModifies(stmtNum, p, q);
            }
        }
    }
    processedProcedures.erase(p);
}

void Calls::storeUsesAndModifies(StmtNum stmtNum, ProcID p, ProcID q) {
//    E.g.
//       procedure p {
//    1.     if (c == d) { --> Uses(1, b), Modifies(1, a)
//    2.         call q; --> Uses(p, b), Modifies(p, a), Uses(2, b), Modifies(2, a)
//           }
//       }
//
//       procedure q {
//    3.     a = b + 1; --> Uses(q, b), Modifies(q, a), Uses(3, b), Modifies(3, a)
//       }

    // Get the variables that are modified by the callee, q
    unordered_set<VarID> variablesModified = PKB::modifies->getVarsModifiedByProc(q);
    unordered_set<VarID> variablesUsed = PKB::uses->getVarsUsedByProc(q);

    // Add Modifies(p, variable) and Modifies(stmtNum, variable) for all variables modified by q.
    for (VarID variable : variablesModified) {
        PKB::modifies->storeProcModifies(p, variable);
        PKB::modifies->storeStmtModifies(stmtNum, variable);
    }

    // Add Uses(p, variable) and Uses(stmtNum, variable) for all variables used by q.
    for (VarID variable : variablesUsed) {
        PKB::uses->storeProcUses(p, variable);
        PKB::uses->storeStmtUses(stmtNum, variable);
    }

    // All parent*s of this Calls statement also modifies/uses the variables modified/used by q.
    unordered_set<StmtNum> parentStars = PKB::parent->getParentStar(stmtNum);
    for (StmtNum stmt : parentStars) {
        for (VarID variable : variablesModified) {
            PKB::modifies->storeStmtModifies(stmt, variable);
        }
        for (VarID variable : variablesUsed) {
            PKB::uses->storeStmtUses(stmt, variable);
        }
    }
}

