#pragma once

#include "../AbstractAPI.h"

class AffectsBip {
public:
    // Constructor for AffectsBip
    AffectsBip();

    // Returns true if AffectsBip(a1, a2)
    bool isAffectsBip(StmtNum a1, StmtNum a2);

    // Returns true if AffectsBip*(a1, a2)
    bool isAffectsBipStar(StmtNum a1, StmtNum a2);

    // Returns a2's such that AffectsBip(a1, a2)
    unordered_set<StmtNum> const &getAffectsBip(StmtNum a1) const;

    // Returns a1's such that AffectsBip(a1, a2)
    unordered_set<StmtNum> const &getAffectedBip(StmtNum a2) const;

    // Returns a2's such that AffectsBip*(a1, a2)
    unordered_set<StmtNum> const &getAffectsBipStar(StmtNum a1) const;

    // Returns a1's such that AffectsBip*(a1, a2)
    unordered_set<StmtNum> const &getAffectedBipStar(StmtNum a2) const;

    // Returns a pair of vectors in the affectsBipMap.
    // First vector is vector of a1's. Second is vector of a2's.
    // e.g. if (1,2), (3,4), (5,6) is in affectsBipMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllAffectsBip();

    // Returns a pair of vectors in the affectsBipStarMap.
    // First vector is vector of a1's. Second is vector of a2's.
    // e.g. if (1,2), (3,4), (5,6) is in affectsBipStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllAffectsBipStar();

    // Returns the no. of pairs of AffectsBip relationship.
    // E.g. if the affectsBipMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getAffectsBipSize();

    // Returns the no. of pairs of AffectsBip* relationship.
    // E.g. if the affectsBipStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getAffectsBipStarSize();

    // Compute the AffectsBip and AffectsBip star relationship.
    void populateAffectsBipAndAffectsBipStar();

    // to switch on/off population of AffectsBip and AffectsBip* relationship
    void setRunAffectsBip(bool runAffectsBip);

    // returns whether we are populating AffectsBip and AffectsBip* relationship
    bool getRunAffectsBip();

private:
    // Stores a1 as key, set of a2's as value for AffectsBip relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > affectsBipMap;

    // Stores a2 as key, set of a1's as value for AffectsBip relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseAffectsBipMap;

    // Stores a1 as key, set of a2's as value for AffectsBip* relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > affectsBipStarMap;

    // Stores a2 as key, set of a1's as value for AffectsBip* relationship
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseAffectsBipStarMap;

    // Maps (n1, branchStack1) to a set of (n2, branchStack2)
    // Each string is n followed by branchStack separated by whitespace
    unordered_map<STRING, unordered_set<STRING> > affectsBipWithBranchStackMap;

    // Stores <a1, a2> in the affectsBipMap
    // Stores <a2, a1> in the reverseAffectsBipMap
    // Returns true if the information is successfully added to the PKB.
    bool storeAffectsBip(StmtNum a1, StmtNum a2);

    // Stores <a1, a2> in the affectsBipStarMap
    // Stores <a2, a1> in the reverseAffectsBipStarMap
    // Returns true if the information is successfully added to the PKB.
    bool storeAffectsBipStar(StmtNum a1, StmtNum a2);

    // Stores <s1, s2> in the affectsBipWithBranchStackMap
    void storeAffectsBipWithBranchStack(STRING s1, STRING s2);

    // Checks that there exists a path from a1 to a2 such that along the path, v is not modified.
    bool pathDoesNotModifyWithBranchStack(STRING s1, STRING s2, ID v, unordered_set<STRING> visited);

    // extract the ProgLine (the first number before first space) from s
    ProgLine findN(STRING s);

    // populate affectsBipWithBranchStackMap and affectsBipMap
    void populateAffectsBip();

    // Use dfs to to find the path taken and populate the affectsBipStarMap
    void populateAffectsBipStar();
    void dfs(STRING source, StmtNum sourceStmt, STRING prev, unordered_set<STRING>& visited);

    // switch to populate AffectsBip and AffectsBip*
    bool runAffectsBip = true;

};
