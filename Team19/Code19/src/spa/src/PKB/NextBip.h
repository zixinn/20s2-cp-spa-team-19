#pragma once

#include "../AbstractAPI.h"

class NextBip {
public:
    NextBip();

    bool isNextBip(ProgLine n1, ProgLine n2);
    bool isNextBipStar(ProgLine n1, ProgLine n2);

    unordered_set<ProgLine> const &getNextBip(ProgLine n1) const;
    unordered_set<ProgLine> const &getNextBipStar(ProgLine n1) const;

    void populateNextBipAndNextBipStar();

private:
    struct hash_pair {
        template <class T1, class T2>
        size_t operator()(const pair<T1, T2>& p) const{
            auto hash1 = hash<T1>{}(p.first);
            auto hash2 = hash<T2>{}(p.second);
            return hash1 ^ hash2;
        }
    };

    int dummyNum = 0;

    // similar to nextMap but with dummy nodes if last stmt in procedure is call
    unordered_map<ProgLine, unordered_set<ProgLine>> nextWithDummyMap;

    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipWithDummyMap;
    // stores edges of cfg
    // maps <start, end> to set of values,
    // where val is 0 if not branch, +prog_line if branch in and -prog_line if branch back
    unordered_map<pair<ProgLine, ProgLine>, ProgLine, hash_pair> cfgBipMap;

    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipMap;
    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipStarMap;

    void storeNextBip(ProgLine n1, ProgLine n2);

    void populateNextWithDummy();
    void populateNextBip();
    void populateNextBipStar();
};
