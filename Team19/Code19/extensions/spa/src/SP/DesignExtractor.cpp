#include "DesignExtractor.h"
#include "SP/Token.h"

// DE internal structures
ProcID DesignExtractor::currentProcedureID;
StmtNum DesignExtractor::currentParent;
set<VarID> DesignExtractor::currentModifiedVarsLst;
set<VarID> DesignExtractor::currentUsedVarsLst;
vector<StmtNum> DesignExtractor::currentStmtLst;
vector<set<StmtNum>> DesignExtractor::currentNext;
// Stacks
vector<vector<StmtNum>> DesignExtractor::stmtLstsStack;
vector<set<VarID>> DesignExtractor::usesStack;
vector<set<VarID>> DesignExtractor::modifiesStack;
vector<StmtNum> DesignExtractor::parentStack;
vector<vector<set<StmtNum>>> DesignExtractor::nextStack;

// To support generic stacks
template <typename T>
T DesignExtractor::DEStack<T>::stackPop(vector<T> &stack) {
    if (stack.empty()) {
        throw std::out_of_range("DE: attempted to pop an empty stack.");
    }
    T entry = stack.back();  // as pop_back() doesn't have return value
    stack.pop_back();
    return entry;
}
template <typename T>
void DesignExtractor::DEStack<T>::stackPush(vector<T> &stack, T entry) {
    stack.push_back(entry);
}

// DE public API for Parser
void DesignExtractor::storeNewProcedure(STRING procedureName) {
    // Stores new procedure into PKB and receive the PKB-assigned ID
    currentProcedureID = PKB::procTable->storeProcName(procedureName);
    createNewCurrentState(NULL); // no parent for a new procedure!
}

void DesignExtractor::exitProcedure() {
    // Store Procedure's stmtLst
    PKB::procTable->storeProcStmt(currentProcedureID, currentStmtLst[0], currentStmtLst.back());
    addCurrentStmtLstToPKB();

    // Update Follows, Modifies, Uses for all statements in this procedure's currentStmtLst
    // Stmts in containers were handled earlier; this includes Parents
    // Uses/Modifies relationships in containers wont be added during exitProcedure
    // because the container statements' stmtLsts are not in the Procedure's currentStmtLst
    addFollowsForCurrentStmtLst();
    // Stores this Procedure's Modifies, Uses relationships for the currentStmtLst
    addAllCurrentStmtLstModifiesForProcedure();
    addAllCurrentStmtLstUsesForProcedure();
    addNextForCurrentStmtLst();
}

void DesignExtractor::storeNewWhile(StmtNum startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts, WhileStmt* AST) {
    // Store the conditional variables into PKB and receive the PKB-assigned ID
    for (STRING varName : condVarNames) {
        VarID varID = PKB::varTable->storeVarName(varName);
        PKB::uses->storeStmtUses(startStmtNum, varID);
        PKB::stmtTable->storeWhilePattern(startStmtNum, varID);
        // DE Internal Bookkeeping
        currentUsedVarsLst.insert(varID);   // the conditional variables used
    }
    for (STRING constant : condConsts) {
        PKB::constTable->storeConst(constant);
    }

    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(startStmtNum, AST, WHILE_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << startStmtNum << " in PKB.\n";
    }

    // DE Internal Bookkeeping
    currentStmtLst.push_back(startStmtNum);
    // Prepare for the while loop's stmtLst
    currentParent = startStmtNum;
    currentNext.push_back(set<ProgLine>{ startStmtNum });
    saveCurrentState();
    createNewCurrentState(startStmtNum); // current While stmt is not part of the new stmtLst

    // This is to account for the Next relationship from while loop to its first nested statement
    currentNext.push_back(set<ProgLine>{ startStmtNum });
}

void DesignExtractor::exitWhile() {
    // Note that the last stmt in While's stmtLst will always go to the while loop itself next
    // So only need to account for branching at the start of the loop
    currentNext.push_back(currentNext.front());
    storeCurrentStmtLstRelationships(); // Store While loop's stmtLst's relationships
    popSavedState();                    // Reset to previous local state variables
}

void DesignExtractor::storeNewIf(StmtNum startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts, IfStmt* AST) {
    // Store the conditional variables into PKB and receive the PKB-assigned ID
    for (STRING varName : condVarNames) {
        VarID varID = PKB::varTable->storeVarName(varName);
        PKB::uses->storeStmtUses(startStmtNum, varID);
        PKB::stmtTable->storeIfPattern(startStmtNum, varID);
        // DE Internal Bookkeeping
        currentUsedVarsLst.insert(varID);   // the conditional variables used
    }
    for (STRING constant : condConsts) {
        PKB::constTable->storeConst(constant);
    }
    // Stores <stmtNum, PAIR<variable ID, *AST>> into StmtAST Map.
    if (!PKB::stmtTable->storeStmt(startStmtNum, AST, IF_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << startStmtNum << " in PKB.\n";
    }
    // DE Internal Bookkeeping
    currentStmtLst.push_back(startStmtNum);
    // Prepare for the If stmt's stmtLst
    currentParent = startStmtNum;

    currentNext.push_back(set<ProgLine>{ startStmtNum });
    currentNext.push_back(set<ProgLine>{ });
    // For the Else portion of the statement later.
    currentNext.push_back(set<ProgLine>{ startStmtNum });

    saveCurrentState();
    createNewCurrentState(startStmtNum); // current If stmt is not part of the new stmtLst
    // This is to account for the Next relationship from the If to its first Then nested statement
    currentNext.push_back(set<ProgLine>{ startStmtNum });
}
void DesignExtractor::storeNewElse() {
    // Stores the else section of if-else statement
    // An if-then and its else statement has their own separate stmtLsts
    // So, it's necessary to populate the if-part abstractions and reset the local state variables
    storeCurrentStmtLstRelationships();
    StmtNum startStmt = currentStmtLst.front();

    // Last stmt in currentNext is the last statement in 'then'
    // Thus, its Next relationship is with the stmt after the IfElse statement.
    set<ProgLine> lastThenStmt = currentNext.back();
    PKB::stmtTable->storeIfStmt(parentStack.back(), startStmt, lastThenStmt);
    createNewCurrentState(currentParent);

    vector<set<ProgLine>> storedNextEntries = DEStack<vector<set<ProgLine>>>::stackPop(nextStack);
    currentNext =  vector<set<ProgLine>>{ storedNextEntries.back() };
    storedNextEntries.pop_back();

    // To store the ProgLine of the branching Then
    storedNextEntries.push_back(lastThenStmt);
    DEStack<vector<set<ProgLine>>>::stackPush(nextStack, storedNextEntries);

}

void DesignExtractor::endIfElse() {
    // Called when exiting an if-else statement.
    storeCurrentStmtLstRelationships(); // Store If stmt's stmtLst's relationships
    StmtNum startStmt = currentStmtLst.front();
    set<ProgLine> lastElseStmt = currentNext.back();
    PKB::stmtTable->storeElseStmt(parentStack.back(), startStmt, lastElseStmt);
    popSavedState();                    // Reset to previous local state variables

    // Add the ProgLine of the branching Else to the ProgLine of the branching Then
    for (int e: lastElseStmt) {
        currentNext.back().insert(e);   // add to the previous currentNext
    }
}

void DesignExtractor::storeNewAssignment(StmtNum stmtNum, STRING variableName, AssignStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    VarID varID = PKB::varTable->storeVarName(variableName);
    // Update Modifies
    PKB::modifies->storeStmtModifies(stmtNum, varID);

    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, ASSIGN_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }

    // Traverse AST and extract the variables/constants used in the Expr
    vector<STRING> varNameLst;
    vector<STRING> constLst;
    Expr* expression = AST->getExpr();
    // AssignStmt contains Expr; getExpr();
    std::tie(varNameLst, constLst) = extractVarsAndConsts(expression, varNameLst, constLst);
    // Store constants into PKB
    for (STRING constant : constLst) {
        PKB::constTable->storeConst(constant);
    }
    // Store variables into PKB & update Uses table.
    for (STRING var : varNameLst) {
        VarID listVarID =  PKB::varTable->storeVarName(var);   // if var already exists, it returns previously assigned ID.
        PKB::uses->storeStmtUses(stmtNum, listVarID);

        // DE Internal Bookkeeping
        currentUsedVarsLst.insert(listVarID);
    }

    // Store <stmtNum, PAIR<variable ID, expr>> into Assignment Expression Map.
    if (!PKB::stmtTable->storeAssignExpr(stmtNum, AST->getName()->getVal(), expression->toString())) {
        std::cerr << "DE encountered an error when attempting to store assignment expression " << stmtNum << " in PKB.\n";
    }

    // DE Internal Bookkeeping
    currentStmtLst.push_back(stmtNum);
    currentModifiedVarsLst.insert(varID);
    currentNext.push_back(set<ProgLine>{ stmtNum });
}

void DesignExtractor::storeNewRead(StmtNum stmtNum, STRING variableName, ReadStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    VarID varID = PKB::varTable->storeVarName(variableName);
    // Update Modifies
    PKB::modifies->storeStmtModifies(stmtNum, varID);

    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, READ_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }

    // Stores <stmtNum, readVarID> into readVariablesMap.
    if (!PKB::stmtTable->storeReadVariableForStmt(stmtNum, varID)) {
        std::cerr << "DE encountered an error when attempting to store read variable for statement " << stmtNum << " in PKB.\n";
    }

    // DE Internal Bookkeeping
    currentStmtLst.push_back(stmtNum);
    currentModifiedVarsLst.insert(varID);
    currentNext.push_back(set<ProgLine>{ stmtNum });
}

void DesignExtractor::storeNewPrint(StmtNum stmtNum, STRING variableName, PrintStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    VarID varID = PKB::varTable->storeVarName(variableName);
    // Update Uses. Note that Print only uses one variable.
    PKB::uses->storeStmtUses(stmtNum, varID);
    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, PRINT_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }

    // Stores <stmtNum, printVarID> into readVariablesMap.
    if (!PKB::stmtTable->storePrintVariableForStmt(stmtNum, varID)) {
        std::cerr << "DE encountered an error when attempting to store print variable for statement " << stmtNum << " in PKB.\n";
    }

    // DE Internal Bookkeeping
    currentStmtLst.push_back(stmtNum);
    currentUsedVarsLst.insert(varID);
    currentNext.push_back(set<ProgLine>{ stmtNum });
}

bool DesignExtractor::storeNewCall(StmtNum stmtNum, STRING callerName, STRING procedureName, CallStmt* AST) {
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, CALL_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }
    // Get procedure IDs
    ProcID callerProcID = PKB::procTable->getProcID(callerName);    // this should be in PKB already
    ProcID calledProcID = PKB::procTable->storeProcName(procedureName);
    if (callerProcID < 0) {
        std::cerr << "DE could not find the callerName " << callerName << " in PKB.\n";
    }

    // Bookkepping
    currentStmtLst.push_back(stmtNum);
    currentNext.push_back(set<ProgLine>{ stmtNum });

    // Only store if this is not a recursive call
    if (callerProcID != calledProcID) {
        return PKB::calls->storeCalls(stmtNum, callerProcID, calledProcID);
    } else {
        return false;
    }
}

void DesignExtractor::signalReset() {
    // Reset DE internal structures
    currentProcedureID = NULL;
    currentParent = NULL;
    currentModifiedVarsLst.clear();
    currentUsedVarsLst.clear();
    currentStmtLst.clear();
    currentNext.clear();
    // Stacks
    stmtLstsStack.clear();
    usesStack.clear();
    modifiesStack.clear();
    parentStack.clear();
    nextStack.clear();
    PKB::resetPKB();
}

bool DesignExtractor::signalEnd() {
    return PKB::populatePKB();
}

// DE Internal Methods
pair<vector<STRING>, vector<STRING>> DesignExtractor::extractVarsAndConsts(Expr* expression,
                                                                           vector<STRING> &varNameLst,
                                                                           vector<STRING> &constLst) {
    // The first in the pair is a list of varNames. The second is a list of Constant strings.
    // 3 types of expressions: InfixExpr, ConstVal, VarName
    sp::Token::TokenType tokenType = expression->getToken()->getType();
    switch (tokenType) {
        // ConstVal
        case sp::Token::TokenType::CONST:
            constLst.push_back(expression->getTokenLiteral());
            break;
            // VarName
        case sp::Token::TokenType::NAME:
            varNameLst.push_back(dynamic_cast<VarName *>(expression)->getVal());
            break;
            // InfixExpr. Fallthrough!
        case sp::Token::TokenType::PLUS:
        case sp::Token::TokenType::MINUS:
        case sp::Token::TokenType::TIMES:
        case sp::Token::TokenType::DIV:
        case sp::Token::TokenType::MOD: {
            auto *infixExpression = dynamic_cast<InfixExpr *>(expression);
            // Traverse left first
            extractVarsAndConsts(infixExpression->getLeft(), varNameLst, constLst);
            extractVarsAndConsts(infixExpression->getRight(), varNameLst, constLst);
            break;
        }
        default:
            throw std::invalid_argument("Expression Node has the wrong token value");
    }
    return make_pair(varNameLst, constLst);
}

void DesignExtractor::storeCurrentStmtLstRelationships() {
    addCurrentStmtLstToPKB();
    // Stores Follows, Parent for the currentStmtLst
    addFollowsForCurrentStmtLst();
    addParentForCurrentStmtLst();
    // Stores Modifies, Uses for the currentStmtLst
    // This is done for ALL parents of the currentStmtLst
    addModifiesForAllParentsOfCurrentStmtLst();
    addUsesForAllParentForCurrentStmtLst();
    // Stores the current Procedure's Modifies, Uses relationships for the currentStmtLst
    // Uses/Modifies relationships in the container wont be added during exitProcedure
    // because the container statements' stmtLsts are not in the Procedure's currentStmtLst
    addAllCurrentStmtLstModifiesForProcedure();
    addAllCurrentStmtLstUsesForProcedure();
    addNextForCurrentStmtLst();
}

void DesignExtractor::addNextForCurrentStmtLst() {
    if (!currentNext.empty()) {
        for (int i = 0; i < currentNext.size() - 1; i++) {
            for (ProgLine n1 : currentNext[i]) {
                for (ProgLine n2 : currentNext[i + 1]) {
                    PKB::next->storeNext(n1, n2);
                }
            }
        }
    }
}

void DesignExtractor::addCurrentStmtLstToPKB() {
    // Adds the container stmt's stmtLst to PKB
    if (!currentStmtLst.empty()) {
        PKB::stmtLstTable->storeStmtLst(currentStmtLst[0]);
    }
}
void DesignExtractor::addFollowsForCurrentStmtLst() {
    // If currentStmtLst contains statements, add Follows(s1, s2) between each
    if (!currentStmtLst.empty()) {
        // size() - 1 because last stmt will have nothing following it
        for (StmtNum i = 0; i < currentStmtLst.size() - 1; i++){
            PKB::follows->storeFollows(currentStmtLst[i], currentStmtLst[i + 1]);
        }
    }
}

void DesignExtractor::addParentForCurrentStmtLst() {
    // adds Parents relationship for all stmts in currentStmtLst
    if (!parentStack.empty()) {
        StmtNum parent = parentStack.back();
        for (StmtNum child : currentStmtLst) {
            PKB::parent->storeParent(parent, child);
        }
    }
}

void DesignExtractor::addModifiesForAllParentsOfCurrentStmtLst() {
    for (VarID varID : currentModifiedVarsLst) {
        for (StmtNum parent : parentStack) {
            PKB::modifies->storeStmtModifies(parent, varID);
        }
    }
}

void DesignExtractor::addUsesForAllParentForCurrentStmtLst() {
    for (VarID varID : currentUsedVarsLst) {
        for (StmtNum parent : parentStack) {
            PKB::uses->storeStmtUses(parent, varID);
        }
    }
}

void DesignExtractor::addAllCurrentStmtLstModifiesForProcedure() {
    for (VarID varID : currentModifiedVarsLst) {
        PKB::modifies->storeProcModifies(currentProcedureID, varID);
    }
}

void DesignExtractor::addAllCurrentStmtLstUsesForProcedure() {
    for (VarID varID : currentUsedVarsLst) {
        PKB::uses->storeProcUses(currentProcedureID, varID);
    }
}

void DesignExtractor::saveCurrentState() {
    // Push all current state variables to the storage stacks.
    DEStack<vector<StmtNum>>::stackPush(stmtLstsStack, currentStmtLst);
    DEStack<StmtNum>::stackPush(parentStack, currentParent);
    DEStack<set<VarID>>::stackPush(usesStack, currentUsedVarsLst);
    DEStack<set<VarID>>::stackPush(modifiesStack, currentModifiedVarsLst);
    DEStack<vector<set<ProgLine>>>::stackPush(nextStack, currentNext);
}

void DesignExtractor::popSavedState() {
    // Pop all storage stacks and restore the current state variables
    currentStmtLst = DEStack<vector<StmtNum>>::stackPop(stmtLstsStack);
    currentParent = DEStack<StmtNum>::stackPop(parentStack);
    currentUsedVarsLst = DEStack<set<VarID>>::stackPop(usesStack);
    currentModifiedVarsLst = DEStack<set<VarID>>::stackPop(modifiesStack);
    currentNext = DEStack<vector<set<ProgLine>>>::stackPop(nextStack);
}

void DesignExtractor::createNewCurrentState(StmtNum parentStmt) {
    currentParent = parentStmt;
    currentStmtLst = vector<StmtNum>();
    currentUsedVarsLst = set<VarID>();
    currentModifiedVarsLst = set<VarID>();
    currentNext = vector<set<ProgLine>>();
}

