#pragma once

#include "../AbstractAPI.h"

class Follows {
public:
    // Constructor for Follows
    Follows();

    // Returns true if Follows(s1, s2)
    bool isFollows(StmtNum s1, StmtNum s2);

    // Returns true if Follows*(s1, s2)
    bool isFollowsStar(StmtNum s1, StmtNum s2);

    // Returns s2, the statement that follows s1. Returns -1 if there is no such statement.
    StmtNum getFollower(StmtNum s1);

    // Returns s1, the statement that is followed by s2. Returns -1 if there is no such statement.
    StmtNum getFollowee(StmtNum s2);

    // Returns set of s2, the statements that Follows*(s1, s2).
    unordered_set<StmtNum> const & getFollowerStar(StmtNum s1) const;

    // Returns set of s1, the statements that Follows*(s1, s2).
    unordered_set<StmtNum> const &getFolloweeStar(StmtNum s2) const;

    // Returns a pair of vectors in the followsMap.
    // First vector is vector of s1's. Second is vector of s2's
    // e.g. if (1,2), (3,4), (5,6) is in followsMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllFollows();

    // Returns a pair of vectors in the followsStarMap.
    // First vector is vector of s1's. Second is vector of s2's
    // e.g. if (1,2), (3,4), (5,6) is in followsStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<StmtNum>, vector<StmtNum> > getAllFollowsStar();

    // Returns the number of entries in followsMap
    int getFollowsSize();

    // Returns the no. of pairs of Follows* relationship.
    // E.g. if the followsStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getFollowsStarSize();

    // Stores <s1, s2> in the followsMap.
    // Stores <s2, s1> in reverseFollowsMap
    // Returns true if info is successfully added.
    bool storeFollows(StmtNum s1, StmtNum s2);

    // Returns true if there is a statement s2 such that Follows(s1, s2)
    bool hasFollower(StmtNum s1) const;

    // Returns true if there is a statement s1 such that Follows(s1, s2)
    bool hasFollowee(StmtNum s2) const;

    // calculate all Follows* relationships from the FollowsMap using implicit dfs and populate the FollowsStarMap and ReverseFollowsStarMap
    void populateFollowsStar();

private:
    // Stores <s1, s2> where Follows(s1, s2)
    unordered_map<StmtNum, StmtNum> followsMap;

    // Stores <s2, s1> where Follows(s1, s2)
    unordered_map<StmtNum, StmtNum> reverseFollowsMap;

    // Stores <s1, set of s2's> where Follows*<s1, s2);
    unordered_map<StmtNum, unordered_set<StmtNum> > followsStarMap;

    // Stores <s2, set of s1's> where Follows*<s1, s2);
    unordered_map<StmtNum, unordered_set<StmtNum> > reverseFollowsStarMap;

    // Stores <s1, s2> in the followsStarMap.
    // Stores <s2, s1> in reverseFollowsStarMap
    // Returns true if info is successfully added.
    bool storeFollowsStar(StmtNum s1, StmtNum s2);

};

