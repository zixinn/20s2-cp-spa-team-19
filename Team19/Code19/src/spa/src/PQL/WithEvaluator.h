#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

class WithEvaluator {
public:
	WithEvaluator();

	static bool evaluate(unordered_map<string, string> declarations,
		Clause clause, unordered_map<string, vector<int>>& tempResults);

	~WithEvaluator();
private:
	static unordered_map<string, string> getArgumentMap(string arg, unordered_map<string, string> declarations);
	static void storeResults(vector<int> results, unordered_map<string, string> argMap,
		unordered_map<string, vector<int>>& tempResults);
	static vector<int> getValues(unordered_map<string, string> argMap);
	static string trimQuotes(string s);
	static bool compareNames(unordered_map<string, string> firstArgMap, unordered_map<string, string> secondArgMap,
		unordered_map<string, vector<int>>& tempResults);
	static vector<int> compareOneArgTypeName(string name, unordered_map<string, string> argMap);
	static string getName(unordered_map<string, string> argMap, int num);
};
