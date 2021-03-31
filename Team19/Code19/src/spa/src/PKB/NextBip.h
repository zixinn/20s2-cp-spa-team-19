#pragma once

#include "../AbstractAPI.h"

#include <stack>

class NextBip {
public:
    NextBip();

    // Returns true if NextBip(n1,n2)
    bool isNextBip(ProgLine n1, ProgLine n2);

    // Returns true if NextBip*(n1,n2)
    bool isNextBipStar(ProgLine n1, ProgLine n2);

    // Returns n2's such that NextBip(n1, n2)
    unordered_set<ProgLine> const &getNextBip(ProgLine n1) const;

    // Returns n1's such that NextBip(n1, n2)
    unordered_set<ProgLine> const & getPreviousBip(ProgLine n2) const;

    // Returns n2's such that NextBip*(n1, n2)
    unordered_set<ProgLine> const &getNextBipStar(ProgLine n1) const;

    // Returns n1's such that NextBip*(n1, n2)
    unordered_set<ProgLine> const & getPreviousBipStar(ProgLine n2) const;

    // Returns a pair of vectors in the nextBipMap.
    // First vector is vector of n1's. Second is vector of n2's.
    // e.g. if (1,2), (3,4), (5,6) is in nextBipMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ProgLine>, vector<ProgLine> > getAllNextBip();

    // Returns a pair of vectors in the nextBipStarMap.
    // First vector is vector of n1's. Second is vector of n2's.
    // e.g. if (1,2), (3,4), (5,6) is in nextBipStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ProgLine>, vector<ProgLine> > getAllNextBipStar();

    // Returns the no. of pairs of NextBip relationship.
    // E.g. if the nextBipMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getNextBipSize();

    // Returns the no. of pairs of NextBip* relationship.
    // E.g. if the nextBipStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getNextBipStarSize();

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

    // similar to nextMap but with dummy nodes
    unordered_map<ProgLine, unordered_set<ProgLine>> nextWithDummyMap;

    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipWithDummyMap;

    // stores edges of cfg
    // maps <start, end> to set of values,
    // where val is 0 if not branch, +prog_line if branch in and -prog_line if branch back
    unordered_map<pair<ProgLine, ProgLine>, ProgLine, hash_pair> cfgBipMap;

    // Maps (n1, branchStack1) to a set of (n2, branchStack2), includes dummy
    // Each string is n followed by branchStack separated by whitespace
    unordered_map<string, unordered_set<string>> nextBipWithBranchStackMap;

    // Stores n1 as key, set of n2's as value for NextBip relationship
    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipMap;

    // Stores n2 as key, set of n1's as value for NextBip relationship
    unordered_map<ProgLine, unordered_set<ProgLine>> reverseNextBipMap;

    // Stores n1 as key, set of n2's as value for NextBip* relationship
    unordered_map<ProgLine, unordered_set<ProgLine>> nextBipStarMap;

    // Stores n2 as key, set of n1's as value for NextBip* relationship
    unordered_map<ProgLine, unordered_set<ProgLine>> reverseNextBipStarMap;

    // Stores <n1, n2> in the nextBipMap.
    void storeNextBip(ProgLine n1, ProgLine n2);

    // Stores <n1, n2> in the nextBipStarMap.
    void storeNextBipStar(ProgLine n1, ProgLine n2);

    // Stores <s1, s2> in the nextBipWithBranchStackMap.
    void storeNextBipWithBranchStack(string s1, string s2);

    // Returns n2's such that NextBipWithDummy(n1, n2)
    unordered_set<ProgLine> const &getNextBipWithDummy(ProgLine n1) const;

    unordered_set<string> const &getNextBipWithBranchStack(string s1) const;

    // populate nextWithDummyMap by adding dummy nodes to nextMap
    void populateNextWithDummy();

    void populateNextBip();

    // Stores <n2, n1> in the reverseNextBipMap.
    void populateReverseNextBip();

    void populateNextBipWithBranchStack();

    void dfs(ProgLine n1, string branchStack, unordered_set<string> visited);

    void populateNextBipStar();

    // Stores <n2, n1> in the reverseNextBipStarMap.
    void populateReverseNextBipStar();

    ProgLine findN(string s);
};
