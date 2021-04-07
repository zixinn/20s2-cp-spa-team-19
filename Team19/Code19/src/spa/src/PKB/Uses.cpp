#include "Uses.h"

Uses::Uses() = default;

bool Uses::stmtUsesVar(StmtNum stmtNum, VarID varID) {
    unordered_map<StmtNum, unordered_set<VarID> >::const_iterator result = stmtUsesMap.find(stmtNum);
    if (result == stmtUsesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

bool Uses::procUsesVar(ProcID procID, VarID varID) {
    unordered_map<ProcID, unordered_set<VarID> >::const_iterator result = procUsesMap.find(procID);
    if (result == procUsesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

const unordered_set<VarID> & Uses::getVarsUsedByStmt(StmtNum stmtNum) const {
    auto result = stmtUsesMap.find(stmtNum);
    if (result == stmtUsesMap.end()) {
        static unordered_set<VarID> empty = unordered_set<VarID>({});
        return empty;
    }
    return result->second;
}

const unordered_set<VarID> & Uses::getVarsUsedByProc(ProcID procID) const {
    auto result = procUsesMap.find(procID);
    if (result == procUsesMap.end()) {
        static unordered_set<VarID> empty = unordered_set<VarID>({});
        return empty;
    }
    return result->second;
}

unordered_set<StmtNum> const &Uses::getStmtsUses(VarID varID) const {
    auto result = reverseStmtUsesMap.find(varID);
    if (result == reverseStmtUsesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return result->second;
}

unordered_set<StmtNum> const &Uses::getProcsUses(VarID varID) const {
    auto result = reverseProcUsesMap.find(varID);
    if (result == reverseProcUsesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return result->second;
}

pair<vector<StmtNum>, vector<VarID>> Uses::getAllStmtUses() {
    vector<StmtNum> stmtNums;
    vector<VarID> varIDs;
    for (auto &it : stmtUsesMap) {
        for (VarID varID : it.second) {
            stmtNums.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(stmtNums, varIDs);
}

pair<vector<ProcID>, vector<VarID>> Uses::getAllProcUses() {
    vector<ProcID> procIDs;
    vector<VarID> varIDs;
    for (auto &it : procUsesMap) {
        for (VarID varID : it.second) {
            procIDs.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(procIDs, varIDs);
}

int Uses::getStmtSize() {
    int cnt = 0;
    for (auto &it : stmtUsesMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Uses::getProcSize() {
    int cnt = 0;
    for (auto &it : procUsesMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Uses::storeStmtUses(StmtNum stmtNum, VarID varID) {
    if (stmtUsesVar(stmtNum, varID)) {
        return false;
    }

    auto it = stmtUsesMap.find(stmtNum);
    if (it == stmtUsesMap.end()) {
        stmtUsesMap.insert({stmtNum, unordered_set<VarID>({varID})});
    } else {
        it->second.insert(varID);
    }

    it = reverseStmtUsesMap.find(varID);
    if (it == reverseStmtUsesMap.end()) {
        reverseStmtUsesMap.insert({varID, unordered_set<StmtNum>({stmtNum})});
    } else {
        it->second.insert(stmtNum);
    }

    return true;
}

bool Uses::storeProcUses(ProcID procID, VarID varID) {
    if (procUsesVar(procID, varID)) {
        return false;
    }

    auto it = procUsesMap.find(procID);
    if (it == procUsesMap.end()) {
        procUsesMap.insert({procID, unordered_set<VarID>({varID})});
    } else {
        it->second.insert(varID);
    }

    it = reverseProcUsesMap.find(varID);
    if (it == reverseProcUsesMap.end()) {
        reverseProcUsesMap.insert({varID, unordered_set<ProcID>({procID})});
    } else {
        it->second.insert(procID);
    }

    return true;
}
