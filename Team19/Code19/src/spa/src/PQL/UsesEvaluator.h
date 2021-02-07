#include "QueryEvaluator.h"
#include "QueryPreprocessor.h"

class UsesEvaluator {
public:
	UsesEvaluator();
	bool evaluate(unordered_map<string, string> declarations,
		Clause clause, unordered_map<string, vector<int>>& tempResults);
	~UsesEvaluator();
private:
	vector<string> validArgTypes;
};