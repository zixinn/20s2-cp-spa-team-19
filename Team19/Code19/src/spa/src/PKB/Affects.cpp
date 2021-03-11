#include "Affects.h"
#include "PKB.h"

Affects::Affects() = default;

bool Affects::isAffects(StmtNum a1, StmtNum a2) {
    unordered_map<StmtNum, unordered_set<StmtNum>>::const_iterator result = affectsMap.find(a1);
    return result != affectsMap.end() && result->second.find(a2) != result->second.end();
}

bool Affects::isAffectsStar(StmtNum a1, StmtNum a2) {
    unordered_map<StmtNum, unordered_set<StmtNum>>::const_iterator result = affectsStarMap.find(a1);
    return result != affectsStarMap.end() && result->second.find(a2) != result->second.end();
}

unordered_set<StmtNum> const &Affects::getAffects(StmtNum a1) const {
    if (affectsMap.find(a1) == affectsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return affectsMap.find(a1)->second;
}

unordered_set<StmtNum> const &Affects::getAffected(StmtNum a2) const {
    if (reverseAffectsMap.find(a2) == reverseAffectsMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseAffectsMap.find(a2)->second;
}

unordered_set<StmtNum> const &Affects::getAffectsStar(StmtNum a1) const {
    if (affectsStarMap.find(a1) == affectsStarMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return affectsStarMap.find(a1)->second;
}

unordered_set<StmtNum> const &Affects::getAffectedStar(StmtNum a2) const {
    if (reverseAffectsStarMap.find(a2) == reverseAffectsStarMap.end()) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseAffectsStarMap.find(a2)->second;
}

pair<vector<StmtNum>, vector<StmtNum> > Affects::getAllAffects() {
    vector<StmtNum> a1s, a2s;
    for (auto &it : affectsMap) {
        for (ID n2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(n2);
        }
    }
    return make_pair(a1s, a2s);
}

pair<vector<StmtNum>, vector<StmtNum> > Affects::getAllAffectsStar() {
    vector<StmtNum> a1s, a2s;
    for (auto &it : affectsStarMap) {
        for (ID n2 : it.second) {
            a1s.push_back(it.first);
            a2s.push_back(n2);
        }
    }
    return make_pair(a1s, a2s);
}

int Affects::getAffectsSize() {
    int cnt = 0;
    for (auto &it : affectsMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Affects::getAffectsStarSize() {
    int cnt = 0;
    for (auto &it : affectsStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Affects::storeAffects(StmtNum a1, StmtNum a2) {
    if (isAffects(a1, a2)) {
        return false;
    }

    auto it = affectsMap.find(a1);
    if (it == affectsMap.end()) {
        affectsMap.insert({a1, unordered_set<StmtNum>({a2})});
    } else {
        it->second.insert(a2);
    }

    it = reverseAffectsMap.find(a2);
    if (it == reverseAffectsMap.end()) {
        reverseAffectsMap.insert({a2, unordered_set<StmtNum>({a1})});
    } else {
        it->second.insert(a1);
    }

    return true;
}

bool Affects::storeAffectsStar(StmtNum a1, StmtNum a2) {
    if (isAffectsStar(a1, a2)) {
        return false;
    }

    auto it = affectsStarMap.find(a1);
    if (it == affectsStarMap.end()) {
        affectsStarMap.insert({a1, unordered_set<StmtNum>({a2})});
    } else {
        it->second.insert(a2);
    }

    it = reverseAffectsStarMap.find(a2);
    if (it == reverseAffectsStarMap.end()) {
        reverseAffectsStarMap.insert({a2, unordered_set<StmtNum>({a1})});
    } else {
        it->second.insert(a1);
    }

    return true;
}

void Affects::populateAffectsAndAffectsStar() {
    populateAffects();
    populateAffectsStar();
}

void Affects::populateAffects() {
    // Get all the assignment statements
    // For every pair of assignment statements a1 and a2 that has a control flow path [isNext*(a1, a2)], find the variable v such that Modifies(a1,v) and check that stmtUsesVar(a2, v)
    // Search for all possible paths to find a path that does not modify v along the way. [in pathDoesNotModify()]
    // If found a path, storeAffects(a1, a2)
    ID v;
    for (StmtNum a1 : PKB::stmtTable->getAllAssignStmtNums()) {
        for (StmtNum a2 : PKB::stmtTable->getAllAssignStmtNums()) {
            if (!PKB::next->isNextStar(a1, a2)) {
                // No control flow path between a1 and a2
                continue;
            }
            v = *PKB::modifies->getVarsModifiedByStmt(a1).begin();
            if (!(PKB::uses->stmtUsesVar(a2, v))) {
                // a1 does not affect a2 since a2 does not use the variable modified by a1
                continue;
            }

            for (ProgLine next : PKB::next->getNext(a1)) {
                if (pathDoesNotModify(next, a2, v)) {
                    storeAffects(a1, a2);
                    break;
                }
            }
        }
    }
}

bool Affects::pathDoesNotModify(StmtNum a1, StmtNum a2, ID v) {
    if (a1 == a2) {
        // we found our way to a2 means we found the path that has not modified v
        return true;
    }

    if (!PKB::next->isNextStar(a1, a2)) {
        // not a path to a2
        return false;
    }

    // if it's container statement, we skip, just check the children using Next later
    // Because Modifies(while/if, v) will be true if any child modifies v.
    if (!PKB::parent->hasChild(a1)) {
        if (PKB::modifies->stmtModifiesVar(a1, v)) {
            return false;
        }
    }

    // A possible path, need to continue checking
    for (ProgLine next : PKB::next->getNext(a1)) {
        if (pathDoesNotModify(next, a2, v)) {
            return true;
        }
    }

    return false;
}

void Affects::populateAffectsStar() {
    unordered_set<StmtNum> a2s;
    ID curr;
    for (auto &it : affectsMap) {
        curr = it.first;
        list<StmtNum> queue;
        unordered_set<StmtNum> processedStmts;
        processedStmts.insert(curr);
        queue.push_back(curr);
        while (!queue.empty()) {
            a2s = getAffects(queue.front());
            queue.pop_front();
            for (StmtNum a2 : a2s) {
                storeAffectsStar(curr, a2);
                if (curr != a2 && processedStmts.find(a2) == processedStmts.end()) {
                    processedStmts.insert(a2);
                    queue.push_back(a2);
                }
            }
        }
    }
}
