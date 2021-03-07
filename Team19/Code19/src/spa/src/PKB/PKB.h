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

#include "../AbstractAPI.h"

using namespace std;
//typedef short PROC;

//class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer
//class ProcTable;

class PKB {
public:
//	static int setProcToAST(PROC p, TNode* r);
//	static TNode* getRootAST (PROC p);

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

    // Resets the entire PKB (delete everything stored in PKB)
    static void resetPKB();

    //  Populate Follows* / Parent*. Returns true if the PKB has been successfully populated.
    static bool populatePKB();

};