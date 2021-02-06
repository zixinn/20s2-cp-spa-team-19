#pragma once

#include "../AbstractAPI.h"

class Parent {
public:
    // Constructor for Parent
    Parent();

    // Returns true if Parent(s1, s2)
    bool isParent(StmtNum s1, StmtNum s2);

    // Returns true if Parent*(s1, s2)
    bool isParentStar(StmtNum s1, StmtNum s2);

    // Returns s2, the statement that is the child of s1. Returns -1 if there is no such statement.
    StmtNum getChild(StmtNum s1);

    // Returns s1, the statement that is the parent of by s2. Returns -1 if there is no such statement.
    StmtNum getParent(StmtNum s2);

    // Returns set of s2, the statements that Parent*(s1, s2).
    unordered_set<StmtNum> const & getChildStar(StmtNum s1) const;

    // Returns set of s1, the statements that Parent*(s1, s2).
    unordered_set<StmtNum> const &getParentStar(StmtNum s2) const;

    // Returns a pair of vectors in the parentMap.
    // First vector is vector of s1's. Second is vector of s2's
    // e.g. if (1,2), (3,4), (5,6) is in parentMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllParent();

    // Returns a pair of vectors in the parentStarMap.
    // First vector is vector of s1's. Second is vector of s2's
    // e.g. if (1,2), (3,4), (5,6) is in parentStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllParentStar();

    // Returns the number of entries in parentMap
    int getParentSize();

    // Returns the no. of pairs of Parent* relationship.
    // E.g. if the parentStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getParentStarSize();

    // Stores <s1, s2> in the parentMap.
    // Stores <s2, s1> in reverseParentMap
    // Returns true if info is successfully added.
    bool storeParent(StmtNum s1, StmtNum s2);

    // Stores <s1, s2> in the parentStarMap.
    // Stores <s2, s1> in reverseParentStarMap
    // Returns true if info is successfully added.
    bool storeParentStar(StmtNum s1, StmtNum s2);

    // Returns true if there is a statement s2 such that Parent(s1, s2)
    bool hasChild(StmtNum s1) const;

    // Returns true if there is a statement s1 such that Parent(s1, s2)
    bool hasParent(StmtNum s2) const;

    // calculate all Parent* relationships from the ParentMap, then populate the ParentStarMap and ReverseParentStarMap
    void populateParentStar();

private:

    // Stores <s1, s2> where Parent(s1, s2)
    unordered_map<StmtNum, StmtNum> parentMap;

    // Stores <s2, s1> where Parent(s1, s2)
    unordered_map<StmtNum, StmtNum> reverseParentMap;

    // Stores <s1, set of s2's> where Parent*<s1, s2);
    unordered_map<StmtNum, unordered_set<StmtNum> > parentStarMap;

    // Stores <s2, set of s1's> where Parent*<s1, s2);
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseParentStarMap;
};

