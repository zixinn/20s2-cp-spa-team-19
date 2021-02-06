#include "Follows.h"

Follows::Follows() = default;

bool Follows::isFollows(StmtNum s1, StmtNum s2) {
    unordered_map<StmtNum, StmtNum>::const_iterator result = followsMap.find(s1);
    return result != followsMap.end() && result->second == s2;
}

bool Follows::isFollowsStar(StmtNum s1, StmtNum s2) {
    unordered_map<StmtNum, unordered_set<StmtNum> >::const_iterator result = followsStarMap.find(s1);
    if (result == followsStarMap.end()) {
        return false;
    }
    return result->second.find(s2) != result->second.end();
}


StmtNum Follows::getFollower(StmtNum s1) {
    if (!hasFollower(s1)) {
        return -1;
    }
    return followsMap.find(s1)->second;
}

StmtNum Follows::getFollowee(StmtNum s2) {
    if (!hasFollowee(s2)) {
        return -1;
    }
    return reverseFollowsMap.find(s2)->second;
}

unordered_set<StmtNum> const &Follows::getFollowerStar(StmtNum s1) const{
    if (!hasFollower(s1)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return followsStarMap.find(s1)->second;
}

unordered_set<StmtNum> const &Follows::getFolloweeStar(StmtNum s2) const {
    if (!hasFollowee(s2)) {
        static unordered_set<StmtNum> empty = unordered_set<StmtNum>({});
        return empty;
    }
    return reverseFollowsStarMap.find(s2)->second;
}

pair<vector<StmtNum>, vector<StmtNum> > Follows::getAllFollows() {
    vector<StmtNum> s1s, s2s;
    for (auto &it : followsMap) {
        s1s.push_back(it.first);
        s2s.push_back(it.second);
    }
    return make_pair(s1s, s2s);
}

pair<vector<StmtNum>, vector<StmtNum> > Follows::getAllFollowsStar() {
    vector<StmtNum> s1s, s2s;
    for (auto &it : followsStarMap) {
        for (StmtNum s2 : it.second) {
            s1s.push_back(it.first);
            s2s.push_back(s2);
        }
    }
    return make_pair(s1s, s2s);
}

int Follows::getFollowsSize() {
    return followsMap.size();
}

int Follows::getFollowsStarSize() {
    int cnt = 0;
    for (auto &it : followsStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

bool Follows::storeFollows(StmtNum s1, StmtNum s2) {
    if (hasFollower(s1)) {
        return false;
    }
    followsMap.insert({s1, s2});
    reverseFollowsMap.insert({s2, s1});
    return true;
}

void Follows::populateFollowsStar() {
    StmtNum s1;
    StmtNum s2;
    StmtNum curr;
    for (auto &it : followsMap) {
        curr = it.first;
        s1 = curr;
        s2 = it.second;
        do {
            storeFollowsStar(curr, s2);
            s1 = s2;
            s2 = getFollower(s1);
        } while (s2 != -1);
    }
}

bool Follows::hasFollower(StmtNum s1) const {
    return followsMap.find(s1) != followsMap.end();
}

bool Follows::hasFollowee(StmtNum s2) const {
    return reverseFollowsMap.find(s2) != reverseFollowsMap.end();
}

bool Follows::storeFollowsStar(StmtNum s1, StmtNum s2) {
    if (isFollowsStar(s1, s2)) {
        return false;
    }

    auto it = followsStarMap.find(s1);
    if (it == followsStarMap.end()) {
         followsStarMap.insert({s1, unordered_set<StmtNum>({s2})});
    } else {
        it->second.insert(s2);
    }

    it = reverseFollowsStarMap.find(s2);
    if (it == reverseFollowsStarMap.end()) {
        reverseFollowsStarMap.insert({s2, unordered_set<StmtNum>({s1})});
    } else {
        it->second.insert(s1);
    }

    return true;
}
