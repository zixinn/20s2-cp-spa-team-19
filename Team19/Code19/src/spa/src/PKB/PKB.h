#pragma once

#include "ConstTable.h" // included as .cpp needs constructor
#include "VarTable.h"
#include "ProcTable.h"
#include "StmtTable.h"
#include "StmtLstTable.h"
#include "Follows.h"
#include "Parent.h"
#include "Uses.h"
#include "Modifies.h"
#include "Calls.h"
#include "Next.h"
#include "Affects.h"
#include "NextBip.h"

#include "../AbstractAPI.h"

using namespace std;

class PKB {
public:
    // PKB Components for Program Design Entities
    static ConstTable* constTable;
    static VarTable* varTable;
    static ProcTable* procTable;
    static StmtTable* stmtTable;
    static StmtLstTable* stmtLstTable;

    // PKB Components for Program Design Abstractions
    static Follows* follows;
    static Parent* parent;
    static Uses* uses;
    static Modifies* modifies;
    static Calls* calls;
    static Next* next;
    static Affects* affects;
    static NextBip* nextBip;

    // Resets the entire PKB (delete everything stored in PKB)
    static void resetPKB();

    //  Populate Follows* / Parent*. Returns true if the PKB has been successfully populated.
    static bool populatePKB();

};