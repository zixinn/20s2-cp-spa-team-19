#include "Next.h"

Next::Next() = default;

bool Next::isNext(ProgLine n1, ProgLine n2) {
    unordered_map<ProgLine, unordered_set<ProgLine>>::const_iterator result = nextMap.find(n1);
    return result != nextMap.end() && result->second.find(n2) != result->second.end();
}

bool Next::isNextStar(ProgLine n1, ProgLine n2) {
    unordered_map<ProgLine, unordered_set<ProgLine>>::const_iterator result = nextStarMap.find(n1);
    return result != nextStarMap.end() && result->second.find(n2) != result->second.end();
}

unordered_set<ProgLine> Next::getNext(ProgLine n1) {
    if (nextMap.find(n1) == nextMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return nextMap.find(n1)->second;
}

unordered_set<ProgLine> Next::getPrevious(ProgLine n2) {
    if (reverseNextMap.find(n2) == reverseNextMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return reverseNextMap.find(n2)->second;
}

unordered_set<ProgLine> Next::getNextStar(ProgLine n1) {
    if (nextStarMap.find(n1) == nextStarMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return nextStarMap.find(n1)->second;
}

unordered_set<ProgLine> Next::getPreviousStar(ProgLine n2) {
    if (reverseNextStarMap.find(n2) == reverseNextStarMap.end()) {
        static unordered_set<ProgLine> empty = unordered_set<ProgLine>({});
        return empty;
    }
    return reverseNextStarMap.find(n2)->second;
}

pair<vector<ProgLine>, vector<ProgLine> > Next::getAllNext() {
    vector<ID> n1s, n2s;
    for (auto &it : nextMap) {
        for (ID n2 : it.second) {
            n1s.push_back(it.first);
            n2s.push_back(n2);
        }
    }
    return make_pair(n1s, n2s);
}

pair<vector<ProgLine>, vector<ProgLine> > Next::getAllNextStar() {
    vector<ID> n1s, n2s;
    for (auto &it : nextStarMap) {
        for (ID n2 : it.second) {
            n1s.push_back(it.first);
            n2s.push_back(n2);
        }
    }
    return make_pair(n1s, n2s);
}

bool Next::storeNext(ProgLine n1, ProgLine n2) {
    if (isNext(n1, n2)) {
        return false;
    }

    auto it = nextMap.find(n1);
    if (it == nextMap.end()) {
        nextMap.insert({n1, unordered_set<ProgLine>({n2})});
    } else {
        it->second.insert(n2);
    }

    it = reverseNextMap.find(n2);
    if (it == reverseNextMap.end()) {
        reverseNextMap.insert({n2, unordered_set<ProgLine>({n1})});
    } else {
        it->second.insert(n1);
    }

    return true;
}

int Next::getNextSize() {
    int cnt = 0;
    for (auto &it : nextMap) {
        cnt += it.second.size();
    }
    return cnt;
}

int Next::getNextStarSize() {
    int cnt = 0;
    for (auto &it : nextStarMap) {
        cnt += it.second.size();
    }
    return cnt;
}

void Next::populateNextStar() {
    unordered_set<ProgLine> n2s;
    ID curr;
    for (auto &it : nextMap) {
        curr = it.first;
        list<int> queue;
        queue.push_back(curr);
        while (!queue.empty()) {
            n2s = getNext(queue.front());
            queue.pop_front();
            for (ID n2 : n2s) {
                if (curr == n2) {
                    // Should stop here else we will get into infinite loop
                    break;
                } else {
                    storeNextStar(curr, n2);
                    queue.push_back(n2);
                }
            }
        }
    }
}

bool Next::storeNextStar(ProgLine n1, ProgLine n2) {
    if (isNextStar(n1, n2)) {
        return false;
    }

    auto it = nextStarMap.find(n1);
    if (it == nextStarMap.end()) {
        nextStarMap.insert({n1, unordered_set<ProgLine>({n2})});
    } else {
        it->second.insert(n2);
    }

    it = reverseNextStarMap.find(n2);
    if (it == reverseNextStarMap.end()) {
        reverseNextStarMap.insert({n2, unordered_set<ProgLine>({n1})});
    } else {
        it->second.insert(n1);
    }

    return true;
}
