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

bool NextBip::isNextBipStarWithBranchStack(string s1, string s2) {
    auto result = nextBipStarWithBranchStackMap.find(s1);
    return result != nextBipStarWithBranchStackMap.end() && result->second.find(s2) != result->second.end();
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

unordered_set<string> const & NextBip::getNextBipWithBranchStack(string s1) const {
    auto it = nextBipWithBranchStackMap.find(s1);
    if (it == nextBipWithBranchStackMap.end()) {
        static unordered_set<string> empty = unordered_set<string>({});
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
    auto it = reverseNextBipStarMap.find(n2);
    if (it == reverseNextBipStarMap.end()) {
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

void NextBip::storeNextBipWithBranchStack(string s1, string s2) {
    if (nextBipWithBranchStackMap.find(s1) == nextBipWithBranchStackMap.end()) {
        nextBipWithBranchStackMap[s1] = unordered_set<string>{s2};
    } else {
        nextBipWithBranchStackMap.find(s1)->second.insert(s2);
    }
}

void NextBip::storeNextBipStar(ProgLine n1, ProgLine n2) {
    if (nextBipStarMap.find(n1) == nextBipStarMap.end()) {
        nextBipStarMap[n1] = unordered_set<ProgLine>{n2};
    } else {
        nextBipStarMap.find(n1)->second.insert(n2);
    }
}

void NextBip::storeNextBipStarWithBranchStack(string s1, string s2) {
    if (nextBipStarWithBranchStackMap.find(s1) == nextBipStarWithBranchStackMap.end()) {
        nextBipStarWithBranchStackMap[s1] = unordered_set<string>{s2};
    } else {
        nextBipStarWithBranchStackMap.find(s1)->second.insert(s2);
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
    if (runNextBip) {
        populateNextWithDummy();
        populateNextBip();
        populateReverseNextBip();

        populateNextBipWithBranchStack();
        populateNextBipStar();
        populateReverseNextBipStar();
    }
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
                }
                for (ProgLine n : elseStmtRange.second) {
                    ProgLine dummy = *nextWithDummyMap.find(n)->second.begin();
                    storeNextBip(dummy, n2);
                    cfgBipMap[make_pair(dummy, n2)] = -n1;
                }

            } else if (find(allCallStmts.begin(), allCallStmts.end(), p.second) != allCallStmts.end()) { // last stmt is call stmt
                ProgLine dummy = *nextWithDummyMap.find(p.second)->second.begin();
                storeNextBip(dummy, n2);
                cfgBipMap[make_pair(dummy, n2)] = -n1;

            } else {
                storeNextBip(p.second, n2);
                cfgBipMap[make_pair(p.second, n2)] = -n1;
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

void NextBip::populateNextBipWithBranchStack() {
    vector<ProgLine> allCallStmts = PKB::stmtTable->getAllCallStmtNums();
    vector<ID> procIds = PKB::procTable->getAllProcIDs();
    for (ID id : procIds) {
        ProgLine startProgLine = PKB::procTable->getProcRange(id).first;
        unordered_set<ProgLine> prevBip = getPreviousBip(startProgLine);
        bool skip = false;
        for (ProgLine n : prevBip) {
            if (find(allCallStmts.begin(), allCallStmts.end(), n) != allCallStmts.end()) {
                skip = true;
                break;
            }
        }
        if (!skip) { // only dfs if prevBip is empty or if none of the prevBip is a call stmt
            string branchStack;
            unordered_set<string> visited;
            dfs(startProgLine, branchStack, visited);
        }
    }
}

void NextBip::dfs(ProgLine n1, string branchStack, unordered_set<string> visited) {
    string stack1 = branchStack;
    string s1 = to_string(n1) + stack1;
    visited.insert(s1);
    unordered_set<ProgLine> n2s = getNextBipWithDummy(n1);
    for (ProgLine n2 : n2s) {
        string originalBranchStack = branchStack;
        int val = cfgBipMap.find(make_pair(n1, n2))->second;
        if (val > 0) { // branch in
            branchStack += " " + to_string(val); // stack.push()
            string stack2 = branchStack;
            string s2 = to_string(n2) + stack2;
            storeNextBipWithBranchStack(s1, s2);
            if (visited.find(s2) == visited.end()) {
                dfs(n2, branchStack, visited);
            }
        } else if (val < 0) { // branch back
            if (!branchStack.empty()) { // stack.empty()
                int pos = branchStack.rfind(' ');
                int top = stoi(branchStack.substr(pos + 1)); // stack.top()
                if (top == -val) {
                    branchStack = branchStack.substr(0, pos); // stack.pop()
                    string stack2 = branchStack;
                    string s2 = to_string(n2) + stack2;
                    storeNextBipWithBranchStack(s1, s2);
                    if (visited.find(s2) == visited.end()) {
                        dfs(n2, branchStack, visited);
                    }
                } // else cannot branch back, ignore
            } // stop
        } else { // val == 0, no branch
            string stack2 = branchStack;
            string s2 = to_string(n2) + stack2;
            storeNextBipWithBranchStack(s1, s2);
            if (visited.find(s2) == visited.end()) {
                dfs(n2, branchStack, visited);
            }
        }
        branchStack = originalBranchStack;
    }
}

void NextBip::populateNextBipStar() {
    for (auto& it : nextBipWithBranchStackMap) {
        string curr = it.first;
        ProgLine n1 = findN(curr);
        list<string> queue;
        unordered_set<string> visited;
        visited.insert(curr);
        queue.push_back(curr);
        while (!queue.empty()) {
            unordered_set<string> s2s = getNextBipWithBranchStack(queue.front());
            queue.pop_front();
            for (string s2 : s2s) {
                ProgLine n2 = findN(s2);
                storeNextBipStar(n1, n2);
                storeNextBipStarWithBranchStack(curr, s2);
                if (visited.find(s2) == visited.end()) {
                    visited.insert(s2);
                    queue.push_back(s2);
                }
            }
        }
    }

    // remove dummy
    for (int i = dummyNum; i < 0; i++) {
        nextBipStarMap.erase(i);
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

    nextBipStarWithBranchStackNoDummyMap = nextBipStarWithBranchStackMap;
    unordered_set<string> toRemove;
    for (auto it : nextBipStarWithBranchStackNoDummyMap) {
        if (it.first.find('-') == 0) {
            toRemove.insert(it.first);
            continue;
        }
        unordered_set<string> newS2s;
        for (string s2 : it.second) {
            if (s2.find('-') == string::npos) {
                newS2s.insert(s2);
            }
        }
        nextBipStarWithBranchStackNoDummyMap[it.first] = newS2s;
    }
    for (string s : toRemove) {
        nextBipStarWithBranchStackNoDummyMap.erase(s);
    }
}

ProgLine NextBip::findN(string s) {
    int pos = s.find(' ');
    if (pos == string::npos) {
        return stoi(s);
    } else {
        return stoi(s.substr(0, pos));
    }
}

void NextBip::populateReverseNextBipStar() {
    ProgLine n1;
    for (auto &it : nextBipStarMap) {
        n1 = it.first;
        for (auto n2 : it.second) {
            if (reverseNextBipStarMap.find(n2) == reverseNextBipStarMap.end()) {
                reverseNextBipStarMap[n2] = unordered_set<ProgLine>{n1};
            } else {
                reverseNextBipStarMap.find(n2)->second.insert(n1);
            }
        }
    }
}

unordered_map<string, unordered_set<string>> NextBip::getNextBipStarWithBranchStackNoDummyMap() {
    return nextBipStarWithBranchStackNoDummyMap;
}

void NextBip::setRunNextBip(bool runNextBip) {
    this->runNextBip = runNextBip;
}

bool NextBip::getRunNextBip() {
    return runNextBip;
}
