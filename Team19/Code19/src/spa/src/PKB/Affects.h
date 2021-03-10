#pragma once

#include "../AbstractAPI.h"

class Affects {
public:
    // Constructor for Affects
    Affects();

    // Returns true if Affects(a1, a2)
    bool isAffects(StmtNum a1, StmtNum a2);

    // Returns true if Affects*(a1, a2)
    bool isAffectsStar(StmtNum a1, StmtNum a2);

    // Returns a2's such that Affects(a1, a2)
    unordered_set<StmtNum> const &getAffects(StmtNum a1) const;

    // Returns a1's such that Affects(a1, a2)
    unordered_set<StmtNum> const &getAffected(StmtNum a2) const;

    // Returns a2's such that Affects*(a1, a2)
    unordered_set<StmtNum> const &getAffectsStar(StmtNum a1) const;

    // Returns a1's such that Affects*(a1, a2)
    unordered_set<StmtNum> const &getAffectedStar(StmtNum a2) const;

    // Returns a pair of vectors in the affectsMap.
    // First vector is vector of a1's. Second is vector of a2's.
    // e.g. if (1,2), (3,4), (5,6) is in affectsMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllAffects();

    // Returns a pair of vectors in the affectsStarMap.
    // First vector is vector of a1's. Second is vector of a2's.
    // e.g. if (1,2), (3,4), (5,6) is in affectsStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllAffectsStar();

    // Returns the no. of pairs of Affects relationship.
    // E.g. if the affectsMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getAffectsSize();

    // Returns the no. of pairs of Affects* relationship.
    // E.g. if the affectsStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getAffectsStarSize();

    // Compute the Affects and Affects star relationship after Uses and Modifies have been completely populated.
    void populateAffectsAndAffectsStar();

private:
    // Stores a1 as key, set of a2's as value for Affects relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > affectsMap;

    // Stores a2 as key, set of a1's as value for Affects relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseAffectsMap;

    // Stores a1 as key, set of a2's as value for Affects* relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > affectsStarMap;

    // Stores a2 as key, set of a1's as value for Affects* relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseAffectsStarMap;

    // Stores <a1, a2> in the affectsMap
    // Stores <a2, a1> in the reverseAffectsMap
    // Returns true if the information is successfully added to the PKB.
    bool storeAffects(StmtNum a1, StmtNum a2);

    // Stores <a1, a2> in the affectsStarMap
    // Stores <a2, a1> in the reverseAffectsStarMap
    // Returns true if the information is successfully added to the PKB.
    bool storeAffectsStar(StmtNum a1, StmtNum a2);

    // Checks that there exists a path from a1 to a2 such that along the path, v is not modified.
    bool pathDoesNotModify(StmtNum a1, StmtNum a2, ID v);

    void populateAffects();

    void populateAffectsStar();

};
