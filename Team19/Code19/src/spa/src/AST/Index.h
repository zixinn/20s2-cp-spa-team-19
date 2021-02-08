/*
 * Purpose: Provide a way to include all .h files in /ast namespace
 * NOTE: may be considered bad practice but its easier than having all these includes
*/


#include "Node.h"

#include "Expr.h"
#include "CondExpr.h"
#include "Stmt.h"
#include "StmtLst.h"

#include "InfixExpr.h"

#include "CondExprWrapper.h"
#include "PrefixCondExpr.h"
#include "InfixCondExpr.h"
#include "RelExpr.h"

#include "VarName.h"
#include "ProcName.h"
#include "ConstVal.h"

#include "Proc.h"
#include "Program.h"

#include "AssignStmt.h"
#include "ReadStmt.h"
#include "CallStmt.h"
#include "PrintStmt.h"
#include "WhileStmt.h"
