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
        for (StmtNum a2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(a2);
        }
    }
    return make_pair(a1s, a2s);
}

pair<vector<StmtNum>, vector<StmtNum> > AffectsBip::getAllAffectsBipStar() {
    vector<StmtNum> a1s, a2s;
    for (auto &it : affectsBipStarMap) {
        for (StmtNum a2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(a2);
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

void AffectsBip::storeAffectsBipWithBranchStack(STRING s1, STRING s2) {
    if (affectsBipWithBranchStackMap.find(s1) == affectsBipWithBranchStackMap.end()) {
        affectsBipWithBranchStackMap[s1] = unordered_set<STRING>{s2};
    } else {
        affectsBipWithBranchStackMap.find(s1)->second.insert(s2);
    }
}

void AffectsBip::populateAffectsBipAndAffectsBipStar() {
    if (PKB::nextBip->getRunNextBip() && runAffectsBip) {
        populateAffectsBip();
        populateAffectsBipStar();
    }
}

// For all ProgLines, n1, that are assignment statements and has NextBip,
// we first get all the NextBip, n2, that are also assignment statements
// We check that the AffectsBip(n1, n2) holds, i.e., n2 uses some variable v modified by n1
// We then check that v is not modified along the path, then store the AffectsBip relationship
void AffectsBip::populateAffectsBip() {
    vector<StmtNum> allAssignStmtNums = PKB::stmtTable->getAllAssignStmtNums();
    unordered_map<STRING, unordered_set<STRING>> nextBipStarWithBranchStackNoDummyMap = PKB::nextBip->getNextBipStarWithBranchStackNoDummyMap();
    for (auto& it : nextBipStarWithBranchStackNoDummyMap) {
        STRING s1 = it.first;
        ProgLine n1 = findN(s1);
        if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n1) == allAssignStmtNums.end()) {
            continue;
        }
        for (STRING s2 : it.second) {
            ProgLine n2 = findN(s2);
            if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n2) == allAssignStmtNums.end()) {
                continue;
            }
            VarID v = *PKB::modifies->getVarsModifiedByStmt(n1).begin();
            if (!(PKB::uses->stmtUsesVar(n2, v))) {
                continue;
            }
            unordered_set<STRING> visited;
            for (STRING s : PKB::nextBip->getNextBipWithBranchStack(s1)) {
                if (pathDoesNotModifyWithBranchStack(s, s2, v, visited)) {
                    storeAffectsBipWithBranchStack(s1, s2);
                    storeAffectsBip(n1, n2);
                }
            }
        }
    }
}

bool AffectsBip::pathDoesNotModifyWithBranchStack(STRING s1, STRING s2, VarID v, unordered_set<STRING> visited) {
    visited.insert(s1);
    if (s1 == s2) {
        // we have found our path that has not modified v
        return true;
    }
    if (!PKB::nextBip->isNextBipStarWithBranchStack(s1, s2)) {
        // not a path to s2
        return false;
    }
    ProgLine n1 = findN(s1);
    vector<StmtNum> allAssignStmtNums = PKB::stmtTable->getAllAssignStmtNums();
    if (find(allAssignStmtNums.begin(), allAssignStmtNums.end(), n1) != allAssignStmtNums.end() && PKB::modifies->stmtModifiesVar(n1, v)) {
        // There is an assignment statement along the path that modifies v.
        return false;
    }

    // A possible path, continue checking
    for (STRING s : PKB::nextBip->getNextBipWithBranchStack(s1)) {
        if (visited.find(s) == visited.end()) {
            if (pathDoesNotModifyWithBranchStack(s, s2, v, visited)) {
                return true;
            }
        }
    }
    visited.erase(s1);
    return false;
}

ProgLine AffectsBip::findN(STRING s) {
    int pos = s.find(' ');
    if (pos == STRING::npos) {
        return stoi(s);
    } else {
        return stoi(s.substr(0, pos));
    }
}

void AffectsBip::populateAffectsBipStar() {
    for (auto& it : affectsBipWithBranchStackMap) {
        unordered_set<STRING> visited;
        dfs(it.first, findN(it.first), it.first, visited);
    }
}

void AffectsBip::dfs(STRING source, StmtNum sourceStmt, STRING prev, unordered_set<STRING>& visited) {
    visited.insert(prev);
    auto itA = affectsBipWithBranchStackMap.find(prev);
    if (itA == affectsBipWithBranchStackMap.end()) {
        return;
    }
    for (STRING s2 : itA->second) {
        StmtNum stmt2 = findN(s2);
        storeAffectsBipStar(sourceStmt, stmt2);
        if (visited.find(s2) == visited.end()) {
            dfs(source, sourceStmt, s2, visited);
        }
    }
}

void AffectsBip::setRunAffectsBip(bool runAffectsBip) {
    this->runAffectsBip = runAffectsBip;
}

bool AffectsBip::getRunAffectsBip() {
    return runAffectsBip;
}
