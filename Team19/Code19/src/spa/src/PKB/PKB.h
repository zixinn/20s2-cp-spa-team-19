#pragma once

#include "ConstTable.h" // included as .cpp needs constructor
#include "VarTable.h"
#include "ProcTable.h"

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

    // PKB Components for Program Data Entities
    static ConstTable* constTable;
    static VarTable* varTable;
    static ProcTable* procTable;


};