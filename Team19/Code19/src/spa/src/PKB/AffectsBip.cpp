#include "AffectsBip.h"
#include "PKB.h"

AffectsBip::AffectsBip() = default;

bool AffectsBip::isAffectsBip(StmtNum a1, StmtNum a2) {
    unordered_map<StmtNum, unordered_set<StmtNum>>::const_iterator result = affectsBipMap.find(a1);
    return result != affectsBipMap.end() && result->second.find(a2) != result->second.end();
}

bool AffectsBip::isAffectsBipStar(StmtNum a1, StmtNum a2) {
    unordered_map<StmtNum, unordered_set<StmtNum>>::const_iterator result = affectsBipStarMap.find(a1);
    return result != affectsBipStarMap.end() && result->second.find(a2) != result->second.end();
}

unordered_set<StmtNum> const &AffectsBip::getAffectsBip(StmtNum a1) const {
    if (affectsBipMap.find(a1) == affectsBipMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return affectsBipMap.find(a1)->second;
}

unordered_set<StmtNum> const &AffectsBip::getAffectedBip(StmtNum a2) const {
    if (reverseAffectsBipMap.find(a2) == reverseAffectsBipMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseAffectsBipMap.find(a2)->second;
}

unordered_set<StmtNum> const &AffectsBip::getAffectsBipStar(StmtNum a1) const {
    if (affectsBipStarMap.find(a1) == affectsBipStarMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return affectsBipStarMap.find(a1)->second;
}

unordered_set<StmtNum> const &AffectsBip::getAffectedBipStar(StmtNum a2) const {
    if (reverseAffectsBipStarMap.find(a2) == reverseAffectsBipStarMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseAffectsBipStarMap.find(a2)->second;
}

pair<vector<StmtNum>, vector<StmtNum> > AffectsBip::getAllAffectsBip() {
    vector<StmtNum> a1s, a2s;
    for (auto &it : affectsBipMap) {
        for (ID n2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(n2);
        }
    }
    return make_pair(a1s, a2s);
}

pair<vector<StmtNum>, vector<StmtNum> > AffectsBip::getAllAffectsBipStar() {
    vector<StmtNum> a1s, a2s;
    for (auto &it : affectsBipStarMap) {
        for (ID n2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(n2);
        }
    }
    return make_pair(a1s, a2s);
}

int AffectsBip::getAffectsBipSize() {
    int cnt = 0;
    for (auto &it : affectsBipMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int AffectsBip::getAffectsBipStarSize() {
    int cnt = 0;
    for (auto &it : affectsBipStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool AffectsBip::storeAffectsBip(StmtNum a1, StmtNum a2) {
    if (isAffectsBip(a1, a2)) {
        return false;
    }

    auto it = affectsBipMap.find(a1);
    if (it == affectsBipMap.end()) {
        affectsBipMap.insert({a1, unordered_set<StmtNum>({a2})});
    } else {
        it->second.insert(a2);
    }

    it = reverseAffectsBipMap.find(a2);
    if (it == reverseAffectsBipMap.end()) {
        reverseAffectsBipMap.insert({a2, unordered_set<StmtNum>({a1})});
    } else {
        it->second.insert(a1);
    }

    return true;
}

bool AffectsBip::storeAffectsBipStar(StmtNum a1, StmtNum a2) {
    if (isAffectsBipStar(a1, a2)) {
        return false;
    }

    auto it = affectsBipStarMap.find(a1);
    if (it == affectsBipStarMap.end()) {
        affectsBipStarMap.insert({a1, unordered_set<StmtNum>({a2})});
    } else {
        it->second.insert(a2);
    }

    it = reverseAffectsBipStarMap.find(a2);
    if (it == reverseAffectsBipStarMap.end()) {
        reverseAffectsBipStarMap.insert({a2, unordered_set<StmtNum>({a1})});
    } else {
        it->second.insert(a1);
    }

    return true;
}

void AffectsBip::storeAffectsBipWithBranchStack(string s1, string s2) {
    if (affectsBipWithBranchStackMap.find(s1) == affectsBipWithBranchStackMap.end()) {
        affectsBipWithBranchStackMap[s1] = unordered_set<string>{s2};
    } else {
        affectsBipWithBranchStackMap.find(s1)->second.insert(s2);
    }
}

void AffectsBip::populateAffectsBipAndAffectsBipStar() {
    if (PKB::nextBip->getRunNextBip() && runAffectsBip) {
        populateAffectsBipWithBranchStack();
        populateAffectsBip();
        populateAffectsBipStar();
    }
}

void AffectsBip::populateAffectsBipWithBranchStack() {
    vector<StmtNum> allAssignStmtNums = PKB::stmtTable->getAllAssignStmtNums();
    unordered_map<string, unordered_set<string>> nextBipStarWithBranchStackNoDummyMap = PKB::nextBip->getNextBipStarWithBranchStackNoDummyMap();
    for (auto& it : nextBipStarWithBranchStackNoDummyMap) {
        string s1 = it.first;
        ProgLine n1 = findN(s1);
        if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n1) == allAssignStmtNums.end()) {
            continue;
        }
        for (string s2 : it.second) {
            ProgLine n2 = findN(s2);
            if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n2) == allAssignStmtNums.end()) {
                continue;
            }
            ID v = *PKB::modifies->getVarsModifiedByStmt(n1).begin();
            if (!(PKB::uses->stmtUsesVar(n2, v))) {
                continue;
            }
            unordered_set<string> visited;
            for (string s : PKB::nextBip->getNextBipWithBranchStack(s1)) {
                if (pathDoesNotModifyWithBranchStack(s, s2, v, visited)) {
                    storeAffectsBipWithBranchStack(s1, s2);
                }
            }
        }
    }
}

bool AffectsBip::pathDoesNotModifyWithBranchStack(string s1, string s2, ID v, unordered_set<string> visited) {
    visited.insert(s1);
    if (s1 == s2) {
        return true;
    }
    if (!PKB::nextBip->isNextBipStarWithBranchStack(s1, s2)) {
        return false;
    }
    ProgLine n1 = findN(s1);
    vector<StmtNum> allAssignStmtNums = PKB::stmtTable->getAllAssignStmtNums();
    if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n1) != allAssignStmtNums.end() && PKB::modifies->stmtModifiesVar(n1, v)) {
        return false;
    }
    for (string s : PKB::nextBip->getNextBipWithBranchStack(s1)) {
        if (visited.find(s) == visited.end()) {
            if (pathDoesNotModifyWithBranchStack(s, s2, v, visited)) {
                return true;
            }
        }
    }
    visited.erase(s1);
    return false;
}

ProgLine AffectsBip::findN(string s) {
    int pos = s.find(' ');
    if (pos == string::npos) {
        return stoi(s);
    } else {
        return stoi(s.substr(0, pos));
    }
}

void AffectsBip::populateAffectsBip() {
    for (auto& it : affectsBipWithBranchStackMap) {
        string s1 = it.first;
        ProgLine n1 = findN(s1);
        for (string s2 : it.second) {
            ProgLine n2 = findN(s2);
            storeAffectsBip(n1, n2);
        }
    }
}

void AffectsBip::populateAffectsBipStar() {
    unordered_map<string, unordered_set<string>> nextBipStarWithBranchStackNoDummyMap = PKB::nextBip->getNextBipStarWithBranchStackNoDummyMap();
    for (auto& it : affectsBipWithBranchStackMap) {
        unordered_set<string> visited;
        dfs(it.first, findN(it.first), it.first, visited, nextBipStarWithBranchStackNoDummyMap);
    }
}

void AffectsBip::dfs(string source, StmtNum sourceStmt, string prev, unordered_set<string>& visited, unordered_map<string, unordered_set<string>> nextBipStarWithBranchStackNoDummyMap) {
    visited.insert(prev);
    auto itA = affectsBipWithBranchStackMap.find(prev);
    auto itN = nextBipStarWithBranchStackNoDummyMap.find(prev);
    if (itA == affectsBipWithBranchStackMap.end() || itN == nextBipStarWithBranchStackNoDummyMap.end()) {
        return;
    }
    for (string s2 : itA->second) {
        if (itN->second.find(s2) != itN->second.end()) {
            if (visited.find(s2) == visited.end()) {
                StmtNum stmt2 = findN(s2);
                storeAffectsBipStar(sourceStmt, stmt2);
                dfs(source, sourceStmt, s2, visited, nextBipStarWithBranchStackNoDummyMap);
            }
        }
    }
}
