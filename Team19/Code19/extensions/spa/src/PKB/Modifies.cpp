#include "Modifies.h"

Modifies::Modifies() = default;

bool Modifies::stmtModifiesVar(StmtNum stmtNum, VarID varID) {
    unordered_map<StmtNum, unordered_set<VarID> >::const_iterator result = stmtModifiesMap.find(stmtNum);
    if (result == stmtModifiesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

bool Modifies::procModifiesVar(ProcID procID, VarID varID) {
    unordered_map<ProcID, unordered_set<VarID> >::const_iterator result = procModifiesMap.find(procID);
    if (result == procModifiesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

const unordered_set<VarID> & Modifies::getVarsModifiedByStmt(StmtNum stmtNum) const {
    auto result = stmtModifiesMap.find(stmtNum);
    if (result == stmtModifiesMap.end()) {
        static unordered_set<VarID> empty = unordered_set<VarID>({});
        return empty;
    }
    return result->second;
}

const unordered_set<VarID> & Modifies::getVarsModifiedByProc(ProcID procID) const {
    auto result = procModifiesMap.find(procID);
    if (result == procModifiesMap.end()) {
        static unordered_set<VarID> empty = unordered_set<VarID>({});
        return empty;
    }
    return result->second;
}

unordered_set<StmtNum> const &Modifies::getStmtsModifies(VarID varID) const {
    auto result = reverseStmtModifiesMap.find(varID);
    if (result == reverseStmtModifiesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return result->second;
}

unordered_set<ProcID> const &Modifies::getProcsModifies(VarID varID) const {
    auto result = reverseProcModifiesMap.find(varID);
    if (result == reverseProcModifiesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return result->second;
}

pair<vector<StmtNum>, vector<VarID>> Modifies::getAllStmtModifies() {
    vector<StmtNum> stmtNums;
    vector<VarID> varIDs;
    for (auto &it : stmtModifiesMap) {
        for (VarID varID : it.second) {
            stmtNums.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(stmtNums, varIDs);
}

pair<vector<ProcID>, vector<VarID>> Modifies::getAllProcModifies() {
    vector<ProcID> procIDs;
    vector<VarID> varIDs;
    for (auto &it : procModifiesMap) {
        for (VarID varID : it.second) {
            procIDs.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(procIDs, varIDs);
}

int Modifies::getStmtSize() {
    int cnt = 0;
    for (auto &it : stmtModifiesMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Modifies::getProcSize() {
    int cnt = 0;
    for (auto &it : procModifiesMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Modifies::storeStmtModifies(StmtNum stmtNum, VarID varID) {
    if (stmtModifiesVar(stmtNum, varID)) {
        return false;
    }

    auto it = stmtModifiesMap.find(stmtNum);
    if (it == stmtModifiesMap.end()) {
        stmtModifiesMap.insert({stmtNum, unordered_set<VarID>({varID})});
    } else {
        it->second.insert(varID);
    }

    it = reverseStmtModifiesMap.find(varID);
    if (it == reverseStmtModifiesMap.end()) {
        reverseStmtModifiesMap.insert({varID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool Modifies::storeProcModifies(ProcID procID, VarID varID) {
    if (procModifiesVar(procID, varID)) {
        return false;
    }

    auto it = procModifiesMap.find(procID);
    if (it == procModifiesMap.end()) {
        procModifiesMap.insert({procID, unordered_set<VarID>({varID})});
    } else {
        it->second.insert(varID);
    }

    it = reverseProcModifiesMap.find(varID);
    if (it == reverseProcModifiesMap.end()) {
        reverseProcModifiesMap.insert({varID, unordered_set<VarID>({procID})});
    } else {
        it->second.insert(procID);
    }

    return true;
}
