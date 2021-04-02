#include "PKB.h"
#include "../TNode.h"

// Initialise PKB Design entity storage components
ConstTable* PKB::constTable = new ConstTable();
VarTable* PKB::varTable = new VarTable();
ProcTable* PKB::procTable = new ProcTable();
StmtTable* PKB::stmtTable = new StmtTable();
StmtLstTable* PKB::stmtLstTable = new StmtLstTable();

// Initialise PKB Design abstraction storage components
Follows* PKB::follows = new Follows();
Parent* PKB::parent = new Parent();
Uses* PKB::uses = new Uses();
Modifies* PKB::modifies = new Modifies();
Calls* PKB::calls = new Calls();
Next* PKB::next = new Next();
Affects* PKB::affects = new Affects();
NextBip* PKB::nextBip = new NextBip();

void PKB::resetPKB() {
    delete constTable;
    delete varTable;
    delete procTable;
    delete stmtTable;
    delete stmtLstTable;

    delete follows;
    delete parent;
    delete uses;
    delete modifies;
    delete calls;
    delete next;
    delete affects;
    delete nextBip;

    constTable = new ConstTable();
    varTable = new VarTable();
    procTable = new ProcTable();
    stmtTable = new StmtTable();
    stmtLstTable = new StmtLstTable();

    follows = new Follows();
    parent = new Parent();
    uses = new Uses();
    modifies = new Modifies();
    calls = new Calls();
    next = new Next();
    affects = new Affects();
    nextBip = new NextBip();
}

bool PKB::populatePKB() {
    follows->populateFollowsStar();
    parent->populateParentStar();
    next->populateNextStar();
    bool success = calls->processCalls();
    affects->populateAffectsAndAffectsStar();
    nextBip->populateNextBipAndNextBipStar();
    return success;
}