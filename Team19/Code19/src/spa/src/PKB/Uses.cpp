#include "Uses.h"

Uses::Uses() = default;

bool Uses::stmtUsesVar(StmtNum stmtNum, ID varID) {
    unordered_map<StmtNum, unordered_set<ID> >::const_iterator result = stmtUsesMap.find(stmtNum);
    if (result == stmtUsesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

bool Uses::procUsesVar(ID procID, ID varID) {
    unordered_map<ID, unordered_set<ID> >::const_iterator result = procUsesMap.find(procID);
    if (result == procUsesMap.end()) {
        return false;
    }
    return result->second.find(varID) != result->second.end();
}

unordered_set<ID> const &Uses::getVarsUsedByStmt(StmtNum stmtNum) const {
    auto result = stmtUsesMap.find(stmtNum);
    if (result == stmtUsesMap.end()) {
        static unordered_set<ID> empty = unordered_set<ID>({});
        return empty;
    }
    return result->second;
}

unordered_set<ID> const &Uses::getVarsUsedByProc(ID procID) const {
    auto result = procUsesMap.find(procID);
    if (result == procUsesMap.end()) {
        static unordered_set<ID> empty = unordered_set<ID>({});
        return empty;
    }
    return result->second;
}

unordered_set<StmtNum> const &Uses::getStmtsUses(ID varID) const {
    auto result = reverseStmtUsesMap.find(varID);
    if (result == reverseStmtUsesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<ID>({});
        return empty;
    }
    return result->second;
}

unordered_set<StmtNum> const &Uses::getProcsUses(ID varID) const {
    auto result = reverseProcUsesMap.find(varID);
    if (result == reverseProcUsesMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<ID>({});
        return empty;
    }
    return result->second;
}

pair<vector<StmtNum>, vector<ID> > Uses::getAllStmtUses() {
    vector<StmtNum> stmtNums;
    vector<ID> varIDs;
    for (auto &it : stmtUsesMap) {
        for (ID varID : it.second) {
            stmtNums.push_back(it.first);
            varIDs.push_back(varID);
        }
    }
    return make_pair(stmtNums, varIDs);
}

pair<vector<ID>, vector<ID> > Uses::getAllProcUses() {
    vector<ID> procIDs, varIDs;
    for (auto &it : procUsesMap) {
        for (ID varID : it.second) {
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

bool Uses::storeStmtUses(StmtNum stmtNum, ID varID) {
    if (stmtUsesVar(stmtNum, varID)) {
        return false;
    }

    auto it = stmtUsesMap.find(stmtNum);
    if (it == stmtUsesMap.end()) {
        stmtUsesMap.insert({stmtNum, unordered_set<ID>({varID})});
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

bool Uses::storeProcUses(ID procID, ID varID) {
    if (procUsesVar(procID, varID)) {
        return false;
    }

    auto it = procUsesMap.find(procID);
    if (it == procUsesMap.end()) {
        procUsesMap.insert({procID, unordered_set<ID>({varID})});
    } else {
        it->second.insert(varID);
    }

    it = reverseProcUsesMap.find(varID);
    if (it == reverseProcUsesMap.end()) {
        reverseProcUsesMap.insert({varID, unordered_set<ID>({procID})});
    } else {
        it->second.insert(procID);
    }

    return true;
}
