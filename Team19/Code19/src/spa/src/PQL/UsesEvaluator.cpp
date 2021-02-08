#include <iterator>
#include <set>

#include "UsesEvaluator.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

using namespace std;
// first have to verify which one is first argument: statement (int) or synonym  //no procedure for iteration 1 
// which one is the second argument: synonym (of variable), variable_name (in quotes) or underscore

// first arg:
// if we have stmt num, call &getVarsUsedByStmt() to get all variables
// else if its a synonym
// PAIR<VECTOR<INT>, VECTOR<INT>> getAllStmtUses(); (to get all the stmt nums) ‘stmt’ | ‘print’ | ‘while’ | ‘if’ | ‘assign’ synonym
//
// second arg:
// if synonym (of variable) - get var, add stmt and var into results.
// if variable (in quotes) - compare with second arg, add stmt num into results
// if underscore - return all stmt num

// we can either query for the stmt number, or query for the variable name (that is being used)

UsesEvaluator::UsesEvaluator() {

}

bool UsesEvaluator::evaluate(unordered_map<string, string> declarations, Clause clause, 
	unordered_map<string, vector<int>>& tempResults) {
	string firstArg = clause.getArgs().at(0);
	string firstType = getArgType(firstArg, declarations);
	if (firstType == NAME_ || firstType == PROCEDURE_) {
		return evaluateProcUses(declarations, clause, tempResults);
	}
	else { // firstType == INTEGER_ || firstType == STMT_ || firstType == READ_ || firstType == ASSIGN_ || firstType == CALL_ || firstType == WHILE_ || firstType == IF_
		return evaluateStmtUses(declarations, clause, tempResults);
	}
}

bool UsesEvaluator::evaluateStmtUses(unordered_map<string, string> declarations, 
	Clause clause, unordered_map<string, vector<int>>& tempResults) {
	vector<string> variables;
	vector<StmtNum> stmtNums;
	bool result = true;
	string firstArg = clause.getArgs().at(0);
	string secondArg = clause.getArgs().at(1);
	string firstType = getArgType(firstArg, declarations);
	string secondType = getArgType(secondArg, declarations);
	
	if (firstType == INTEGER_) { // 1, "v" or 1, v or 1, _
		int stmtNum = stoi(firstArg);

		if (secondType == NAME_) { // 1, "v"
			int varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
			if (varId == -1) {
				return false;
			}
			return PKB::uses->stmtUsesVar(stmtNum, varId);
		}
		else { // 1, v or 1, _
			unordered_set<ID> varSet = PKB::uses->getVarsUsedByStmt(stmtNum);
	
			if (varSet.empty()) {
				return false;
			}
			if (secondType != UNDERSCORE_) {
				vector<int> variables;
				variables.assign(varSet.begin(), varSet.end());
				vector<int> res;
				bool nonEmpty = intersectSingleSynonym(variables, selectAll(secondType), res);
				if (nonEmpty) {
					tempResults[secondArg] = res;
				}
				return nonEmpty;
			}
			return true;
		}
	}
	else { // s, "v" or s, v or s, _
		vector<StmtNum> usesStmts = PKB::uses->getAllStmtUses().first; 
		
		if (secondType == NAME_) { // s, "v"
			int varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
			if (varId == -1) {
				return false;
			}
			unordered_set<int> stmts = PKB::uses->getStmtsUses(varId);
			if (stmts.empty()) {
				return false;
			}
			vector<int> statements;
			statements.assign(stmts.begin(), stmts.end());
			vector<int> res;
			bool nonEmpty = intersectSingleSynonym(statements, selectAll(firstType), res);
			if (nonEmpty) {
				tempResults[firstArg] = res;
			}
			return nonEmpty;
		}
		else if (secondType == VARIABLE_) { // s, v or s, _
			pair<vector<int>, vector<int>> allStmtUses = PKB::uses->getAllStmtUses();
			if (allStmtUses.first.empty()) {
				return false;
			}
			if (secondType == UNDERSCORE_) { // s, _
				vector<int> res;
				bool nonEmpty = intersectSingleSynonym(allStmtUses.first, selectAll(firstType), res);
				if (nonEmpty) {
					tempResults[firstArg] = res;
				}
				return nonEmpty;
			}
			else { // s, v
				pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
				pair<vector<int>, vector<int>> res;
				bool nonEmpty = intersectDoubleSynonym(allStmtUses, allCorrectType, res);
				if (nonEmpty) {
					tempResults[firstArg] = res.first;
					tempResults[secondArg] = res.second;
				}
				return nonEmpty;
			}
		}
	}
}

bool UsesEvaluator::evaluateProcUses(unordered_map<string, string> declarations, 
	Clause clause, unordered_map<string, vector<int>>& tempResults) {
	string firstArg = clause.getArgs().at(0);
	string secondArg = clause.getArgs().at(1);
	string firstType = getArgType(firstArg, declarations);
	string secondType = getArgType(secondArg, declarations);

	if (firstType == NAME_) { // "main", "v" or "main", v or "main", _
		int procId = PKB::procTable->getProcID(trim(firstArg.substr(1, firstArg.size() - 2)));
		if (secondType == NAME_) { // "main", "v"
			int varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
			if (procId == -1 || varId == -1) {
				return false;
			}
			return PKB::uses->procUsesVar(procId, varId);

		}
		else { // "main", v or "main", _
			unordered_set<int> vars = PKB::uses->getVarsUsedByProc(procId);
			if (vars.empty()) {
				return false;
			}
			if (secondType != UNDERSCORE_) {
				vector<int> variables;
				variables.assign(vars.begin(), vars.end());
				vector<int> res;
				bool nonEmpty = intersectSingleSynonym(variables, selectAll(secondType), res);
				if (nonEmpty) {
					tempResults[secondArg] = res;
				}
				return nonEmpty;
			}
			return true;
		}

	}
	else { // p, "v" or p, v or p, _
		if (secondType == NAME_) { // p, "v"
			int varId = PKB::varTable->getVarID(trim(secondArg.substr(1, secondArg.size() - 2)));
			if (varId == -1) {
				return false;
			}
			unordered_set<int> procs = PKB::uses->getProcsUses(varId);
			if (procs.empty()) {
				return false;
			}
			vector<int> procedures;
			procedures.assign(procs.begin(), procs.end());
			vector<int> res;
			bool nonEmpty = intersectSingleSynonym(procedures, selectAll(firstType), res);
			if (nonEmpty) {
				tempResults[firstArg] = res;
			}
			return nonEmpty;

		}
		else { // p, v or p, _
			pair<vector<int>, vector<int>> allProcUses = PKB::uses->getAllProcUses();
			if (allProcUses.first.empty()) {
				return false;
			}
			if (secondType == UNDERSCORE_) { // p, _
				vector<int> res;
				bool nonEmpty = intersectSingleSynonym(allProcUses.first, selectAll(firstType), res);
				if (nonEmpty) {
					tempResults[firstArg] = res;
				}
				return nonEmpty;
			}
			else { // p, v
				pair<vector<int>, vector<int>> allCorrectType = make_pair(selectAll(firstType), selectAll(secondType));
				pair<vector<int>, vector<int>> res;
				bool nonEmpty = intersectDoubleSynonym(allProcUses, allCorrectType, res);
				if (nonEmpty) {
					tempResults[firstArg] = res.first;
					tempResults[secondArg] = res.second;
				}
				return nonEmpty;
			}
		}
	}
}

UsesEvaluator::~UsesEvaluator() {

}
