#pragma once

#include "../AbstractAPI.h"

class Next {
public:
    // Constructor for Next
    Next();

    // Returns true if Next(n1,n2)
    bool isNext(ProgLine n1, ProgLine n2);

    // Returns true if Next*(n1, n2)
    bool isNextStar(ProgLine n1, ProgLine n2);

    // Returns n2's such that Next(n1, n2)
    unordered_set<ProgLine> getNext(ProgLine n1);

    // Returns n1's such that Next(n1, n2)
    unordered_set<ProgLine> getPrevious(ProgLine n2);

    // Returns n2's such that Next*(n1, n2)
    unordered_set<ProgLine> getNextStar(ProgLine n1);

    // Returns n1's such that Next*(n1, n2)
    unordered_set<ProgLine> getPreviousStar(ProgLine n2);

    // Returns a pair of vectors in the nextMap.
    // First vector is vector of n1's. Second is vector of n2's.
    // e.g. if (1,2), (3,4), (5,6) is in nextMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ProgLine>, vector<ProgLine> > getAllNext();

    // Returns a pair of vectors in the nextStarMap.
    // First vector is vector of n1's. Second is vector of n2's.
    // e.g. if (1,2), (3,4), (5,6) is in nextStarMap, then it will return <[1,3,5], [2,4,6]>
    pair<vector<ProgLine>, vector<ProgLine> > getAllNextStar();

    // Returns the no. of pairs of Next relationship.
    // E.g. if the nextMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getNextSize();

    // Returns the no. of pairs of Next* relationship.
    // E.g. if the nextStarMap has [1: {2,3}], then the size is 2. Because there are pairs (1,2) and (1,3).
    int getNextStarSize();

    // Stores a Next relationship into the nextMap. Returns true if the information is successfully added to the PKB.
    bool storeNext(ProgLine n1, ProgLine n2);

    // calculate all Next* relationships from the nextMap and populate the nextStarMap and reverseNextStarMap
    void populateNextStar();

private:
    // Stores n1 as key, set of n2's as value for Next relationship
    unordered_map<ProgLine, unordered_set<ProgLine> > nextMap;

    // Stores n2 as key, set of n1's as value for Next relationship
    unordered_map<ProgLine, unordered_set<ProgLine> > reverseNextMap;

    // Stores n1 as key, set of n2's as value for Next* relationship
    unordered_map<ProgLine, unordered_set<ProgLine> > nextStarMap;

    // Stores n2 as key, set of n1's as value for Next* relationship
    unordered_map<ProgLine, unordered_set<ProgLine> > reverseNextStarMap;

    // Stores <n1, n2> in the nextStarMap.
    // Stores <n2, n1> in reverseNextStarMap
    // Returns true if info is successfully added.
    bool storeNextStar(ProgLine n1, ProgLine n2);
};

