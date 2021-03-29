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

unordered_set<ProgLine> const & NextBip::getNextBipWithDummy(ProgLine n1) const {
    auto it = nextBipWithDummyMap.find(n1);
    if (it == nextBipWithDummyMap.end()) {
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

bool NextBip::storeNextBipStar(ProgLine n1, ProgLine n2) {
    if (isNextBipStar(n1, n2)) {
        return false;
    }
    if (nextBipStarMap.find(n1) == nextBipStarMap.end()) {
        nextBipStarMap[n1] = unordered_set<ProgLine>{n2};
    } else {
        nextBipStarMap.find(n1)->second.insert(n2);
    }

    if (reverseNextBipStarMap.find(n2) == reverseNextBipStarMap.end()) {
        reverseNextBipStarMap[n2] = unordered_set<ProgLine>{n1};
    } else {
        reverseNextBipStarMap.find(n2)->second.insert(n1);
    }
    return true;
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

void NextBip::populateNextBipStar() {
    ProgLine curr, n1;
    unordered_set<ProgLine> n2s;
    int branch;

    vector<ProgLine> allIfStmts = PKB::stmtTable->getAllIfStmtNums();
    vector<ProgLine> allWhileStmts = PKB::stmtTable->getAllWhileStmtNums();

    for (auto &it : nextBipWithDummyMap) {
        list<ProgLine> stack;
        list<int> branch_stack;

        unordered_set<ProgLine> processed;
        curr = it.first;
//        cout << endl << "curr: " << curr << endl;

        if (curr < 0) { // ignore dummy nodes, to be removed later
            continue;
        }

        stack.push_back(curr);

        while (!stack.empty()) {
//            cout << "in stack now: ";
//            for (ProgLine progLine : stack) {
//                cout << progLine << " " ;
//            }
//            cout << endl;

            n1 = stack.back();
            stack.pop_back();
            n2s = getNextBipWithDummy(n1);
            processed.insert(n1);

            for (ProgLine n2 : n2s) {
                branch = cfgBipMap.find(make_pair(n1,n2))->second;
//                cout << "n1: " << n1 <<" n2: " << n2 << " branch " << branch << endl;
                if (branch == 0) {
                    // Not branching
//                    cout << "No branching" << endl;

                    if (find(allIfStmts.begin(), allIfStmts.end(), n2) != allIfStmts.end()) {
                        if (!branch_stack.empty()) {
                            branch_stack.push_back(branch_stack.back()); // will need an additional branch-back
                        }
                    }

                    storeNextBipStar(curr, n2);
                    if (find(processed.begin(), processed.end(), n2) == processed.end()) {
                        stack.push_back(n2);
                        processed.insert(n2);
                    }
                } else if (branch > 0) {
                    // Branching in
//                    cout << "Branching in" << endl;

                    branch_stack.push_back(branch);

                    if (find(allIfStmts.begin(), allIfStmts.end(), n2) != allIfStmts.end()) {
                        branch_stack.push_back(branch); // Need additional branch-back
                    }

                    storeNextBipStar(curr, n2);
                    if (find(processed.begin(), processed.end(), n2) == processed.end()) {
                        stack.push_back(n2);
                        processed.insert(n2);
                    }
                } else {
                    // branch < 0 --> branch out
                    if (branch_stack.empty()) {
                        // possible branch. just branch out
//                        cout << "Branching out" << endl;
                        storeNextBipStar(curr, n2);

                        if (find(processed.begin(), processed.end(), n2) == processed.end()) {
                            stack.push_back(n2);
                            processed.insert(n2);
                        }

                    } else if (branch_stack.back() == -branch) {
                        // Correct branch out
//                        cout << "Branching out" << endl;
                        storeNextBipStar(curr, n2);
//                        cout << "Back of stack: " << branch_stack.back() << endl;

                        branch_stack.pop_back();

                        if (find(processed.begin(), processed.end(), n2) == processed.end()) {
                            stack.push_back(n2);
                            processed.insert(n2);
                        }
                        break; // already taken the branch back. move on to the next n1.
                    }
//                    else {
//                        cout << "Back of stack: " << branch_stack.back();
//                        cout << "Not branching out" << endl;
//                    }
                }
            }
            if (find(allWhileStmts.begin(), allWhileStmts.end(), n1) == allWhileStmts.end()) { // if it's not a while statement, we can process it again.
                processed.erase(n1);
            }
        }
    }

    // Delete dummy nodes
    for (auto &it : nextBipStarMap) {
        ProgLine n1 = it.first;
        unordered_set<ProgLine> n2s = it.second;
        for (ProgLine n2 : it.second) {
            if (n2 < 0) {
                n2s.erase(n2);
            }
        }
        nextBipStarMap[n1] = n2s;
    }

}
