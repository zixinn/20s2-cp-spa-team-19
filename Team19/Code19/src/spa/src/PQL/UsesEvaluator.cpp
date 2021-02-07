#include <iterator>
#include <set>

#include "UsesEvaluator.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"
//#include "UsesTable.h"

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

vector<string> validArgTypes = { STMT_, PRINT_, WHILE_, IF_, ASSIGN_ };

bool UsesEvaluator::evaluate(unordered_map<string, string> declarations, 
	Clause clause, unordered_map<string, vector<int>>& tempResults) {
	vector<string> variables;
	vector<StmtNum> stmtNums;
	bool result = true;
	string arg1 = clause.getArgs().at(0);
	string arg2 = clause.getArgs().at(1);
	string arg1Type = getArgType(arg1, declarations);
	string arg2Type = getArgType(arg2, declarations);
	
	if (arg1Type == INTEGER_) { // first arg is an integer
		int stmtNum = stoi(arg1);
		set<int> varSet = PKB::usesTable->getVarsUsedByStmt(stmtNum);
		set<int>::iterator it = varSet.begin();

		while (it != varSet.end())
		{
			string varName = PKB::varTable->getVarName(*it);
			variables.push_back(varName);
			it++;
		}
	}
	else if (find(validArgTypes.begin(), validArgTypes.end(), arg1Type) 
		!= validArgTypes.end()) { // first arg is a synonym
		vector<StmtNum> usesStmts = PKB::usesTable->getAllStmtUses().first; // get all stmts that uses something
		
		if (arg2Type == NAME_) { // if arg 2 is synonym of variable
			stmtNums = usesStmts;
			variables = PKB::usesTable->getAllStmtUses().second;
		}
		else if (arg2Type == VARIABLE_) { // if arg2 is variable in quotes
			if (!PKB::varTable->hasVar(arg2)) {
				return false;
			}

			vector<StmtNum>::iterator it1 = usesStmts.begin();
			int numStmtUseVar = 0;
			while (it1 != usesStmts.end()) { // iterate through all uses stmts
				set<int> varsUsed = PKB::usesTable->getVarsUsedByStmt(*it1); // get all variables used by a stmt
				int varId = PKB::varTable->getVarID(arg2);
				
				if (varsUsed.find(varId) != varsUsed.end()) { // var found in stmt, add stmt to stmt list
					stmtNums.push_back(*it1);
					numStmtUseVar++;
				}
				else { continue; }
				
				it1++;
			}
			if (numStmtUseVar == 0) { result = false; }
		}
		else if (arg2Type == UNDERSCORE_) { stmtNums = usesStmts; }
		else { result = false; }
	}
	else { result = false; }

	return result;
}

UsesEvaluator::~UsesEvaluator() {

}


//assign a; variable v;
//Select a such that Uses(a, v) pattern a(v, _)

//variable v;
//Select v such that Uses(14, v)

//assign a; while w;
//Select a pattern a(“x”, _) such that Uses(a, “x”)

//Uses(6, “c”)

//stmt s; variable v;
//Select s such that Uses(s, v)

