#include "ModifiesEvaluator.h"

using namespace std;

ModifiesEvaluator::ModifiesEvaluator() {

}

bool ModifiesEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING firstType = getArgType(firstArg, declarations);
    if (firstType == NAME_ || firstType == PROCEDURE_) {
        return evaluateProcModifies(declarations, clause, tempResults);
    } else { // firstType == INTEGER_ || firstType == STMT_ || firstType == READ_ || firstType == ASSIGN_ || firstType == CALL_ || firstType == WHILE_ || firstType == IF_
        return evaluateStmtModifies(declarations, clause, tempResults);
    }
}

bool ModifiesEvaluator::evaluateStmtModifies(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == INTEGER_) { // 1, "v" or 1, v or 1, _
        if (secondType == NAME_) { // 1, "v"
            ID varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
            if (varId == -1) {
                return false;
            }
            return PKB::modifies->stmtModifiesVar(stoi(firstArg), varId);

        } else { // 1, v or 1, _
            unordered_set<ID> vars = PKB::modifies->getVarsModifiedByStmt(stoi(firstArg));
            if (vars.empty()) {
                return false;
            }
            if (secondType != UNDERSCORE_) {
                vector<int> res;
                bool nonEmpty = intersectSingleSynonym(vars, selectAll(secondType), res);
                if (nonEmpty) {
                    tempResults[secondArg] = res;
                }
                return nonEmpty;
            }
            return true;
        }

    } else { // s, "v" or s, v or s, _
        if (secondType == NAME_) { // s, "v"
            ID varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
            if (varId == -1) {
                return false;
            }
            unordered_set<StmtNum> stmts = PKB::modifies->getStmtsModifies(varId);
            if (stmts.empty()) {
                return false;
            }
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(stmts, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;

        } else { // s, v or s, _
            pair<vector<StmtNum>, vector<ID>> allStmtModifies = PKB::modifies->getAllStmtModifies();
            if (allStmtModifies.first.empty()) {
                return false;
            }
            if (secondType == UNDERSCORE_) { // s, _
                vector<int> res;
                bool nonEmpty = intersectSingleSynonym(allStmtModifies.first, selectAll(firstType), res);
                if (nonEmpty) {
                    tempResults[firstArg] = res;
                }
                return nonEmpty;
            } else { // s, v
                pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
                pair<vector<int>, vector<int>> res;
                bool nonEmpty = intersectDoubleSynonym(allStmtModifies, allCorrectType, res);
                if (nonEmpty) {
                    tempResults[firstArg] = res.first;
                    tempResults[secondArg] = res.second;
                }
                return nonEmpty;
            }
        }
    }
}

bool ModifiesEvaluator::evaluateProcModifies(unordered_map<STRING, STRING> declarations, Clause clause, unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    STRING firstType = getArgType(firstArg, declarations);
    STRING secondType = getArgType(secondArg, declarations);

    if (firstType == NAME_) { // "main", "v" or "main", v or "main", _
        ID procId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
        if (secondType == NAME_) { // "main", "v"
            ID varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
            if (procId == -1 || varId == -1) {
                return false;
            }
            return PKB::modifies->procModifiesVar(procId, varId);

        } else { // "main", v or "main", _
            unordered_set<ID> vars = PKB::modifies->getVarsModifiedByProc(procId);
            if (vars.empty()) {
                return false;
            }
            if (secondType != UNDERSCORE_) {
                vector<int> res;
                bool nonEmpty = intersectSingleSynonym(vars, selectAll(secondType), res);
                if (nonEmpty) {
                    tempResults[secondArg] = res;
                }
                return nonEmpty;
            }
            return true;
        }

    } else { // p, "v" or p, v or p, _
        if (secondType == NAME_) { // p, "v"
            ID varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
            if (varId == -1) {
                return false;
            }
            unordered_set<ID> procs = PKB::modifies->getProcsModifies(varId);
            if (procs.empty()) {
                return false;
            }
            vector<int> res;
            bool nonEmpty = intersectSingleSynonym(procs, selectAll(firstType), res);
            if (nonEmpty) {
                tempResults[firstArg] = res;
            }
            return nonEmpty;

        } else { // p, v or p, _
            pair<vector<ID>, vector<ID>> allProcModifies = PKB::modifies->getAllProcModifies();
            if (allProcModifies.first.empty()) {
                return false;
            }
            if (secondType == UNDERSCORE_) { // p, _
                vector<int> res;
                bool nonEmpty = intersectSingleSynonym(allProcModifies.first, selectAll(firstType), res);
                if (nonEmpty) {
                    tempResults[firstArg] = res;
                }
                return nonEmpty;
            } else { // p, v
                pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
                pair<vector<int>, vector<int>> res;
                bool nonEmpty = intersectDoubleSynonym(allProcModifies, allCorrectType, res);
                if (nonEmpty) {
                    tempResults[firstArg] = res.first;
                    tempResults[secondArg] = res.second;
                }
                return nonEmpty;
            }
        }
    }
}

ModifiesEvaluator::~ModifiesEvaluator() {

}
