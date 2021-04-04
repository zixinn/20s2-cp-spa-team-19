#pragma once

#include "Clause.h"
#include "QueryUtility.h"
#include "../PKB/PKB.h"

// Helper class to evaluate with statements
class WithEvaluator {
public:
    WithEvaluator();

    static bool evaluate(unordered_map<STRING, STRING> declarations,
                         Clause clause, unordered_map<STRING, vector<int>>& tempResults);

    ~WithEvaluator();
private:
    static unordered_map<STRING, STRING> getArgumentMap(STRING arg, unordered_map<STRING, STRING> declarations);
    static void storeResults(vector<int> results, unordered_map<STRING, STRING> argMap,
                             unordered_map<STRING, vector<int>>& tempResults);
    static vector<int> getValues(unordered_map<STRING, STRING> argMap);
    static STRING trimQuotes(STRING s);
    static bool compareNames(unordered_map<STRING, STRING> firstArgMap, unordered_map<STRING, STRING> secondArgMap,
                             unordered_map<STRING, vector<int>>& tempResults);
    static vector<int> compareOneArgTypeName(STRING name, unordered_map<STRING, STRING> argMap);
    static STRING getName(unordered_map<STRING, STRING> argMap, StmtNum num);
};
