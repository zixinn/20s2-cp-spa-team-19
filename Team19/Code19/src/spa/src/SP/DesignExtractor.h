#include <set>

#include "../AbstractAPI.h"
#include "../AST/Index.h"

#include "PKB/PKB.h"

using namespace std;
using namespace ast;

class DesignExtractor {
public:
    // Methods for Parser
    // Store a new procedure
    static void storeNewProcedure(STRING procedureName);
    // Called when exiting a procedure
    static void exitProcedure();
    // Store a new while statement
    static void storeNewWhile(int startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts,  Stmt* AST);
    // Called when exiting a while loop
    static void exitWhile();

    // The 3 methods below should be called for a single if-else stmt.
    // Stores a new If into the PKB.
    static void storeNewIf(int startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts, Stmt* AST);
    // Stores the else section of the if-else statement
    static void storeNewElse();
    // Called when exiting the if-else statement.
    static void endIfElse();

    // Stores a new Assignment into the PKB.
    static void storeNewAssignment(int stmtNum, STRING variableName, AssignStmt* AST);
    // Stores a new Read into the PKB.
    static void storeNewRead(int stmtNum, STRING variableName, ReadStmt* AST);
    // Stores a new Print into the PKB.
    static void storeNewPrint(int stmtNum, STRING variableName, PrintStmt* AST);

    // Informs DE that end of file (EOF) has been reached.
    // DE will call PKB functions for the calculation of Follows*, Parent*,
    // and (for iteration2/3, Call stmts + Uses/Modifies relationships added by Calls)
    static bool signalEnd();
    // Informs DE that an error was encountered, causing DE and PKB to be reset.
    static void signalReset();

    // To support generic stacks
    template <typename T>
    class DEStack {
    public:
        static T stackPop(vector<T> &stack);
        static void stackPush(vector<T> &stack, T entry);
    };

    // Iteration 2/3
        // Finds or adds the entry of the callerName in the NestedCallsMap and append procedureName to the calledProceduresList of the entry. Returns true (success) if successful; otherwise, returns false if this is a recursive call.
        // bool storeNewCall(int stmtNum,STRING callerName, CallStmt* AST);

private:
    // For bookkeeping
    // "Current statement" refers to the statement that Parser has called DE to process
    // The procedure ID for the procedure containing the current statement
    static ID currentProcedureID;

    // The variables here are all /relative/ to the current statement.
    // The stmtLst for the current statement
    static vector<ID> currentStmtLst;
    // The ID of the (direct) Parent of the current statement
    static ID currentParent;
    // The current list of modified variables for this statement
    static set<ID> currentModifiedVarsLst;
    // The current list of used variables for this statement
    static set<ID> currentUsedVarsLst;
    // Stacks to handle container statements & nested container statements.
    // Each stack stores its respective entries for the CURRENT PROCEDURE (currentProcedureId)
    // e.g. a stack of stmtLsts, a stack of vectors (containing IDs)
    static vector<vector<int>> stmtLstsStack;    // StmtNums
    static vector<set<ID>> usesStack;
    static vector<set<ID>> modifiesStack;
    static vector<int> parentStack;
    // These stacks must always be handled together as a unit.
    static void saveCurrentState(); // push to all stacks
    // Wipes the local state variables for the given parent (container) statement
    static void popSavedState(); // pop all stacks, restoring the current state variables
    static void createNewCurrentState(ID currentParent);

    // Other internal DE methods
    // Extracts varNames and Constants from the RHS of an AssignStmt
    static pair<vector<STRING>, vector<STRING>> extractVarsAndConsts(Expr* AST,
                                                                     vector<STRING> &varNameLst,
                                                                     vector<STRING> &constLst);
    // Stores Follows, Parent, Modifies, Uses for the currentStmtLst (for container statements)
    static void storeCurrentStmtLstRelationships();
    // Stores the first stmt number in the currentStmtLst into PKB's StmtLstTable
    static void addCurrentStmtLstToPKB();
    // Helper methods to store Follows for the currentStmtLst (for container statements)
    static void addFollowsForCurrentStmtLst();
    // Helper method to store Parent for the currentStmtLst (for container statements)
    static void addParentForCurrentStmtLst();
    // Helper method to store Modifies for all Parents of the currentStmtLst (for container statements)
    static void addModifiesForAllParentsOfCurrentStmtLst();
    // Helper method to store Uses for all Parents of the currentStmtLst (for container statements)
    static void addUsesForAllParentForCurrentStmtLst();

    // Helper method to store Modifies for the currentStmtLst (for the current procedure)
    static void addAllCurrentStmtLstModifiesForProcedure();
    // Helper method to store Uses for the currentStmtLst (for the current procedure)
    static void addAllCurrentStmtLstUsesForProcedure();

};


