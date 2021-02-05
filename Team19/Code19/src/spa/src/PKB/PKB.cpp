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

//int PKB::setProcToAST(PROC p, TNode* r) {
//	return 0;
//}
//
//TNode* PKB::getRootAST (PROC p){
//	return nullptr;
//}
