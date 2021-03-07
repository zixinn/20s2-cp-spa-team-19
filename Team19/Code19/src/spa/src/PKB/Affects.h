#pragma once

#include "../AbstractAPI.h"

class Affects {
    // Constructor for Affects
    Affects();

    // Returns true if Affects(a1, a2)
    bool isAffects(StmtNum a1, StmtNum a2);

    // Returns true if Affects*(a1, a2)
    bool isAffectsStar(StmtNum a1, StmtNum a2);



};
