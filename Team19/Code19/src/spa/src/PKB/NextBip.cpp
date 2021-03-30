#include "NextBip.h"
#include "PKB.h"

NextBip::NextBip() = default;

bool NextBip::isNextBip(ProgLine n1, ProgLine n2) {
    auto result = nextBipMap.find(n1);
    return result != nextBipMap.end() && result->second.find(n2) != result->second.end();
}

bool NextBip::isNextBipStar(ProgLine n1, ProgLine n2) {
    auto result = nextBipStarMap.find(n1);
    return result != nextBipStarMap.end() && result->second.find(n2) != result->second.end();
}

unordered_set<ProgLine> const & NextBip::getNextBip(ProgLine n1) const {
    auto it = nextBipMap.find(n1);
    if (it == nextBipMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return it->second;
}

unordered_set<ProgLine> const & NextBip::getPreviousBip(ProgLine n2) const {
    auto it = reverseNextBipMap.find(n2);
    if (it == reverseNextBipMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return it->second;
}

unordered_set<ProgLine> const & NextBip::getNextBipWithDummy(ProgLine n1) const {
    auto it = nextBipWithDummyMap.find(n1);
    if (it == nextBipWithDummyMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return it->second;
}

unordered_set<ProgLine> const & NextBip::getNextBipStar(ProgLine n1) const {
    auto it = nextBipStarMap.find(n1);
    if (it == nextBipStarMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return it->second;
}

unordered_set<ProgLine> const & NextBip::getPreviousBipStar(ProgLine n2) const {
    auto it = reverseNextBipMap.find(n2);
    if (it == reverseNextBipMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return it->second;
}

pair<vector<ProgLine>, vector<ProgLine> > NextBip::getAllNextBip() {
    vector<ProgLine> n1s, n2s;
    for (auto &it : nextBipMap) {
        for (ProgLine n2 : it.second) {
            n1s.push_back(it.first);
            n2s.push_back(n2);
        }
    }
    return make_pair(n1s, n2s);
}

pair<vector<ProgLine>, vector<ProgLine> > NextBip::getAllNextBipStar() {
    vector<ProgLine> n1s, n2s;
    for (auto &it : nextBipStarMap) {
        for (ProgLine n2 : it.second) {
            n1s.push_back(it.first);
            n2s.push_back(n2);
        }
    }
    return make_pair(n1s, n2s);
}

void NextBip::storeNextBip(ProgLine n1, ProgLine n2) {
    if (nextBipMap.find(n1) == nextBipMap.end()) {
        nextBipMap[n1] = unordered_set<ProgLine>{n2};
    } else {
        nextBipMap.find(n1)->second.insert(n2);
    }
}

void NextBip::storeNextBipStar(ProgLine n1, ProgLine n2) {
    if (nextBipStarMap.find(n1) == nextBipStarMap.end()) {
        nextBipStarMap[n1] = unordered_set<ProgLine>{n2};
    } else {
        nextBipStarMap.find(n1)->second.insert(n2);
    }
}

int NextBip::getNextBipSize() {
    int cnt = 0;
    for (auto &it : nextBipMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int NextBip::getNextBipStarSize() {
    int cnt = 0;
    for (auto &it : nextBipStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}


void NextBip::populateNextBipAndNextBipStar() {
    populateNextWithDummy();
    populateNextBip();
    populateReverseNextBip();
    populateNextBipStar();
}

void NextBip::populateNextWithDummy() {
    nextWithDummyMap = PKB::next->getNextMap();
    vector<ProgLine> allProgLines = PKB::stmtTable->getAllStmtNums();
    for (ProgLine n : allProgLines) {
        if (PKB::next->getNext(n).empty()) {
            dummyNum--;
            nextWithDummyMap.insert({n, unordered_set<ProgLine>({dummyNum})});
        }
    }
}

void NextBip::populateNextBip() {
    vector<ProgLine> allProgLines = PKB::stmtTable->getAllStmtNums();
    vector<ProgLine> allCallStmts = PKB::stmtTable->getAllCallStmtNums();
    vector<ProgLine> allIfStmts = PKB::stmtTable->getAllIfStmtNums();

    for (ProgLine n1 : allProgLines) {
        if (find(allCallStmts.begin(), allCallStmts.end(), n1) != allCallStmts.end()) { // n1 is a call stmt
            ID callee = PKB::calls->getCalleeInStmt(n1); // proc id of callee
            pair<ProgLine, ProgLine> p = PKB::procTable->getProcRange(callee); // <startStmt, endStmt>

            // BranchIn
            storeNextBip(n1, p.first);
            cfgBipMap[make_pair(n1, p.first)] = n1;
            branchMap[n1] = make_pair(n1, p.first);

            // BranchBack
            unordered_set<ProgLine> n2s = nextWithDummyMap.find(n1)->second;
            ProgLine n2 = *n2s.begin(); // n2s has only 1 element, may be negative (dummy)
            if (find(allIfStmts.begin(), allIfStmts.end(), p.second) != allIfStmts.end()) { // last stmt is if stmt
                pair<ProgLine, set<ProgLine>> ifStmtRange = PKB::stmtTable->getIfStmtRange(p.second);
                pair<ProgLine, set<ProgLine>> elseStmtRange = PKB::stmtTable->getElseStmtRange(p.second);
                for (ProgLine n : ifStmtRange.second) {
                    ProgLine dummy = *nextWithDummyMap.find(n)->second.begin();
                    storeNextBip(dummy, n2);
                    cfgBipMap[make_pair(dummy, n2)] = -n1;
                    branchMap[-n1] = make_pair(dummy, n2);
                }
                for (ProgLine n : elseStmtRange.second) {
                    ProgLine dummy = *nextWithDummyMap.find(n)->second.begin();
                    storeNextBip(dummy, n2);
                    cfgBipMap[make_pair(dummy, n2)] = -n1;
                    branchMap[-n1] = make_pair(dummy, n2);
                }

            } else if (find(allCallStmts.begin(), allCallStmts.end(), p.second) != allCallStmts.end()) { // last stmt is call stmt
                ProgLine dummy = *nextWithDummyMap.find(p.second)->second.begin();
                storeNextBip(dummy, n2);
                cfgBipMap[make_pair(dummy, n2)] = -n1;
                branchMap[-n1] = make_pair(dummy, n2);

            } else {
                storeNextBip(p.second, n2);
                cfgBipMap[make_pair(p.second, n2)] = -n1;
                branchMap[-n1] = make_pair(p.second, n2);
            }

        } else { // n1 is not a call stmt, NextBip same as Next
            if (nextWithDummyMap.find(n1) != nextWithDummyMap.end()) {
                unordered_set<ProgLine> n2s = nextWithDummyMap.find(n1)->second;
                for (ProgLine n2 : n2s) {
                    storeNextBip(n1, n2);
                    cfgBipMap[make_pair(n1, n2)] = 0;
                }
            }
        }
    }

    nextBipWithDummyMap = nextBipMap;

    // populate nextBip for progline whose nextBip is dummy
    for (auto it : nextBipMap) {
        ProgLine n1 = it.first;
        if (n1 < 0) { // ignore dummy nodes, to be removed later
            continue;
        }

        unordered_set<ProgLine> newN2s;
        for (ProgLine n2 : it.second) {
            if (n2 < 0) { // branchBack to procedure where call is the last stmt
                list<ProgLine> queue;
                queue.push_back(n2);
                while (!queue.empty()) {
                    unordered_set<ProgLine> ns = getNextBip(queue.front());
                    queue.pop_front();
                    for (ProgLine n : ns) {
                        if (n < 0) {
                            queue.push_back(n);
                        } else {
                            newN2s.insert(n);
                        }
                    }
                }
            } else {
                newN2s.insert(n2);
            }
            nextBipMap[n1] = newN2s;
        }
    }

    for (int i = dummyNum; i < 0; i++) {
        nextBipMap.erase(i);
    }
}

void NextBip::populateReverseNextBip() {
    ProgLine n1;
    for (auto &it : nextBipMap) {
        n1 = it.first;
        for (auto n2 : it.second) {
            if (reverseNextBipMap.find(n2) == reverseNextBipMap.end()) {
                reverseNextBipMap[n2] = unordered_set<ProgLine>{n1};
            } else {
                reverseNextBipMap.find(n2)->second.insert(n1);
            }
        }
    }
}

void NextBip::populateNextBipStar() {
    for (auto& it : nextBipWithDummyMap) {
        if (it.first < 0) {
            continue;
        }
        stack<int> branchStack;
        dfs(it.first, it.first, branchStack);
    }

    for (auto& it : nextBipStarMap) {
        unordered_set<ProgLine> newN2s;
        for (ProgLine n2 : it.second) {
            if (n2 > 0) {
                newN2s.insert(n2);
            }
        }
        nextBipStarMap[it.first] = newN2s;
    }
}

void NextBip::dfs(ProgLine source, ProgLine n1, stack<int> branchStack) {
    unordered_set<ProgLine> n2s = getNextBipWithDummy(n1);
    for (ProgLine n2 : n2s) {
        stack<int> originalBranchStack = branchStack;
        int val = cfgBipMap.find(make_pair(n1, n2))->second;
        if (val > 0) { // branch in
            branchStack.push(val);
            storeNextBipStarWithCheck(source, n2, branchStack);
        } else if (val < 0) { // branch back
            if (!branchStack.empty()) {
                if (branchStack.top() == -val) {
                    branchStack.pop();
                    storeNextBipStarWithCheck(source, n2, branchStack);
                } // else cannot branch back, ignore
            } else {
                storeNextBipStarWithCheck(source, n2, branchStack);
            }
        } else { // val == 0, no branch
            storeNextBipStarWithCheck(source, n2, branchStack);
        }
        branchStack = originalBranchStack;
    }
}

void NextBip::storeNextBipStarWithCheck(ProgLine n1, ProgLine n2, stack<int> branchStack) {
    if (isNextBipStar(n1, n2)) {
        if (!branchStack.empty()) {
            pair<ProgLine, ProgLine> p = branchMap.find(branchStack.top())->second;
            branchStack.pop();
            storeNextBipStar(n1, p.second);
            if (n1 != p.second) {
                dfs(n1, p.second, branchStack);
            }
        }
    } else {
        storeNextBipStar(n1, n2);
        if (n1 != n2) {
            dfs(n1, n2, branchStack);
        }
    }
}
