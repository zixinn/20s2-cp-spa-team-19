#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "../TNode.h"

// Initialise PKB entity storage components
ConstTable* PKB::constTable = new ConstTable();
//VarTable* PKB::varTable = new VarTable();
//ProcTable* PKB::procTable = new ProcTable();

// Initialise PKB abstration storage components

//int PKB::setProcToAST(PROC p, TNode* r) {
//	return 0;
//}
//
//TNode* PKB::getRootAST (PROC p){
//	return nullptr;
//}
