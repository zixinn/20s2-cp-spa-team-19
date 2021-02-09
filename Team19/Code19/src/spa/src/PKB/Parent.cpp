#include "Parent.h"

Parent::Parent() = default;

bool Parent::isParent(StmtNum s1, StmtNum s2) {
    unordered_map<StmtNum, unordered_set<StmtNum>>::const_iterator result = parentMap.find(s1);
    return result != parentMap.end() && result->second.find(s2) != result->second.end();
}

bool Parent::isParentStar(StmtNum s1, StmtNum s2) {
    unordered_map<StmtNum, unordered_set<StmtNum> >::const_iterator result = parentStarMap.find(s1);
    if (result == parentStarMap.end()) {
        return false;
    }
    return result->second.find(s2) != result->second.end();
}

unordered_set<StmtNum> const &Parent::getChildren(StmtNum s1) const {
    if (!hasChild(s1)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return parentMap.find(s1)->second;
}

StmtNum Parent::getParent(StmtNum s2) {
    if (!hasParent(s2)) {
        return -1;
    }
    return reverseParentMap.find(s2)->second;
}

unordered_set<StmtNum> const &Parent::getChildrenStar(StmtNum s1) const {
    if (!hasChild(s1)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return parentStarMap.find(s1)->second;
}

unordered_set<StmtNum> const &Parent::getParentStar(StmtNum s2) const {
    if (!hasParent(s2)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseParentStarMap.find(s2)->second;
}

pair<vector<StmtNum>, vector<StmtNum> > Parent::getAllParent() {
    vector<StmtNum> s1s, s2s;
    for (auto &it : parentMap) {
        for (StmtNum s2 : it.second) {
            s1s.push_back(it.first);
            s2s.push_back(s2);
        }
    }
    return make_pair(s1s, s2s);
}

pair<vector<StmtNum>, vector<StmtNum> > Parent::getAllParentStar() {
    vector<StmtNum> s1s, s2s;
    for (auto &it : parentStarMap) {
        for (StmtNum s2 : it.second) {
            s1s.push_back(it.first);
            s2s.push_back(s2);
        }
    }
    return make_pair(s1s, s2s);
}

int Parent::getParentSize() {
    int cnt = 0;
    for (auto &it : parentMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Parent::getParentStarSize() {
    int cnt = 0;
    for (auto &it : parentStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Parent::storeParent(StmtNum s1, StmtNum s2) {
    // Parent can have many children (StmtLst), but each child belongs to 1 parent.
    if (hasParent(s2)) {
        return false;
    }
    if (hasChild(s1)) {
        parentMap.find(s1)->second.insert(s2);
    } else {
        parentMap.insert({s1, unordered_set<StmtNum>({s2})});
    }
    reverseParentMap.insert({s2, s1});
    return true;
}

bool Parent::storeParentStar(StmtNum s1, StmtNum s2) {
    if (isParentStar(s1, s2)) {
        return false;
    }

    auto it = parentStarMap.find(s1);
    if (it == parentStarMap.end()) {
        parentStarMap.insert({s1, unordered_set<StmtNum>({s2})});
    } else {
        it->second.insert(s2);
    }

    it = reverseParentStarMap.find(s2);
    if (it == reverseParentStarMap.end()) {
        reverseParentStarMap.insert({s2, unordered_set<StmtNum>({s1})});
    } else {
        it->second.insert(s1);
    }

    return true;
}

bool Parent::hasChild(StmtNum s1) const {
    return parentMap.find(s1) != parentMap.end();
}

bool Parent::hasParent(StmtNum s2) const {
    return reverseParentMap.find(s2) != reverseParentMap.end();
}

void Parent::populateParentStar() {
    StmtNum s1;
    unordered_set<StmtNum> s2s;
    StmtNum curr;
    for (auto &it : parentMap) {
        curr = it.first;
        list<int> queue;
        queue.push_back(curr);
        while (!queue.empty()) {
            s2s = getChildren(queue.front());
            queue.pop_front();
            for (StmtNum s2 : s2s) {
                storeParentStar(curr, s2);
                queue.push_back(s2);
            }
        }
    }
}