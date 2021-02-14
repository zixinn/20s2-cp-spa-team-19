#include "DesignExtractor.h"
#include "SP/Token.h"

// DE internal structures
ID DesignExtractor::currentProcedureID;
StmtNum DesignExtractor::currentParent;
set<ID> DesignExtractor::currentModifiedVarsLst;
set<ID> DesignExtractor::currentUsedVarsLst;
vector<StmtNum> DesignExtractor::currentStmtLst;
// Stacks
vector<vector<StmtNum>> DesignExtractor::stmtLstsStack;
vector<set<ID>> DesignExtractor::usesStack;
vector<set<ID>> DesignExtractor::modifiesStack;
vector<StmtNum> DesignExtractor::parentStack;

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
}

void DesignExtractor::storeNewWhile(StmtNum startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts, Stmt* AST) {
    // Store the conditional variables into PKB and receive the PKB-assigned ID
    for (STRING varName : condVarNames) {
        ID varID = PKB::varTable->storeVarName(varName);
        PKB::uses->storeStmtUses(startStmtNum, varID);

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
    saveCurrentState();
    createNewCurrentState(startStmtNum); // current While stmt is not part of the new stmtLst
}

void DesignExtractor::exitWhile() {
    storeCurrentStmtLstRelationships(); // Store While loop's stmtLst's relationships
    popSavedState();                    // Reset to previous local state variables
}

void DesignExtractor::storeNewIf(StmtNum startStmtNum, vector<STRING> condVarNames, vector<STRING> condConsts, Stmt* AST) {
    // Store the conditional variables into PKB and receive the PKB-assigned ID
    for (STRING varName : condVarNames) {
        ID varID = PKB::varTable->storeVarName(varName);
         PKB::uses->storeStmtUses(startStmtNum, varID);

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
    saveCurrentState();
    createNewCurrentState(startStmtNum); // current If stmt is not part of the new stmtLst
}
void DesignExtractor::storeNewElse() {
    // Stores the else section of if-else statement
    // An if-then and its else statement has their own separate stmtLsts
    // So, it's necessary to populate the if-part abstractions and reset the local state variables
    storeCurrentStmtLstRelationships();
    createNewCurrentState(currentParent);
}

void DesignExtractor::endIfElse() {
    // Called when exiting an if-else statement.
    storeCurrentStmtLstRelationships(); // Store If stmt's stmtLst's relationships
    popSavedState();                    // Reset to previous local state variables
}

void DesignExtractor::storeNewAssignment(StmtNum stmtNum, STRING variableName, AssignStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    ID varID = PKB::varTable->storeVarName(variableName);
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
        ID listVarID =  PKB::varTable->storeVarName(var);   // if var already exists, it returns previously assigned ID.
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
}

void DesignExtractor::storeNewRead(StmtNum stmtNum, STRING variableName, ReadStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    ID varID = PKB::varTable->storeVarName(variableName);
    // Update Modifies
     PKB::modifies->storeStmtModifies(stmtNum, varID);

    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, READ_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }
    // DE Internal Bookkeeping
    currentStmtLst.push_back(stmtNum);
    currentModifiedVarsLst.insert(varID);
}

void DesignExtractor::storeNewPrint(StmtNum stmtNum, STRING variableName, PrintStmt* AST) {
    // Store the LHS variable into PKB and receive the PKB-assigned ID
    ID varID = PKB::varTable->storeVarName(variableName);
    // Update Uses. Note that Print only uses one variable.
     PKB::uses->storeStmtUses(stmtNum, varID);
    // Stores <stmtNum, PAIR<variable ID, *AST>> into Assignment Map.
    if (!PKB::stmtTable->storeStmt(stmtNum, AST, PRINT_)) {
        std::cerr << "DE encountered an error when attempting to store statement " << stmtNum << " in PKB.\n";
    }
    // DE Internal Bookkeeping
    currentStmtLst.push_back(stmtNum);
    currentUsedVarsLst.insert(varID);
}

void DesignExtractor::signalReset() {
    // Reset DE internal structures
    currentProcedureID = NULL;
    currentParent = NULL;
    currentModifiedVarsLst.clear();
    currentUsedVarsLst.clear();
    currentStmtLst.clear();
    // Stacks
    stmtLstsStack.clear();
    usesStack.clear();
    modifiesStack.clear();
    parentStack.clear();

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
    for (ID varID : currentModifiedVarsLst) {
        for (StmtNum parent : parentStack) {
            PKB::modifies->storeStmtModifies(parent, varID);
        }
    }
}

void DesignExtractor::addUsesForAllParentForCurrentStmtLst() {
    for (ID varID : currentUsedVarsLst) {
        for (StmtNum parent : parentStack) {
            PKB::uses->storeStmtUses(parent, varID);
        }
    }
}

void DesignExtractor::addAllCurrentStmtLstModifiesForProcedure() {
    for (ID varID : currentModifiedVarsLst) {
        PKB::modifies->storeProcModifies(currentProcedureID, varID);
    }
}

void DesignExtractor::addAllCurrentStmtLstUsesForProcedure() {
    for (ID varID : currentUsedVarsLst) {
        PKB::uses->storeProcUses(currentProcedureID, varID);
    }
}

void DesignExtractor::saveCurrentState() {
    // Push all current state variables to the storage stacks.
    DEStack<vector<StmtNum>>::stackPush(stmtLstsStack, currentStmtLst);
    DEStack<StmtNum>::stackPush(parentStack, currentParent);
    DEStack<set<ID>>::stackPush(usesStack, currentUsedVarsLst);
    DEStack<set<ID>>::stackPush(modifiesStack, currentModifiedVarsLst);
}

void DesignExtractor::popSavedState() {
    // Pop all storage stacks and restore the current state variables
    currentStmtLst = DEStack<vector<StmtNum>>::stackPop(stmtLstsStack);
    currentParent = DEStack<StmtNum>::stackPop(parentStack);
    currentUsedVarsLst = DEStack<set<ID>>::stackPop(usesStack);
    currentModifiedVarsLst = DEStack<set<ID>>::stackPop(modifiesStack);
}

void DesignExtractor::createNewCurrentState(ID parentStmt) {
    currentParent = parentStmt;
    currentStmtLst = vector<ID>();
    currentUsedVarsLst = set<ID>();
    currentModifiedVarsLst = set<ID>();
}

