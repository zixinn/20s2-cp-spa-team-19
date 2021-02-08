#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

class UsesEvaluator {
public:
	UsesEvaluator();

	static bool evaluate(unordered_map<string, string> declarations, 
		Clause clause, unordered_map<string, vector<int>>& tempResults);

	~UsesEvaluator();
private:
	static bool evaluateStmtUses(unordered_map<string, string> declarations, 
		Clause clause, unordered_map<string, vector<int>>& tempResults);

	static bool evaluateProcUses(unordered_map<string, string> declarations, 
		Clause clause, unordered_map<string, vector<int>>& tempResults);
};