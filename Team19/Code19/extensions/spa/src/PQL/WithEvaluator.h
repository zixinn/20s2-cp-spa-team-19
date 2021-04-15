#pragma once

#include "QueryUtility.h"

// Helper class to evaluate with clauses
class WithEvaluator {
public:
    // Constructor for WithEvaluator
    WithEvaluator();

    // Evaluates the clause and stores the results in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool evaluate(unordered_map<STRING, STRING> declarations,  Clause clause,
                         unordered_map<STRING, vector<int>>& tempResults);

    // Destructor for WithEvaluator
    ~WithEvaluator();

private:
    // Returns the argument, argument type and attribute name of the argument in the with clause
    static unordered_map<STRING, STRING> getArgumentMap(STRING arg, unordered_map<STRING, STRING> declarations);

    // Stores the results of the with clause with attribute values of type INTEGER in the unordered map tempResults
    static void storeResults(vector<int> results, unordered_map<STRING, STRING> argMap,
                             unordered_map<STRING, vector<int>>& tempResults);

    // Returns a vector of values for the argument represented by the argMap
    static vector<int> getValues(unordered_map<STRING, STRING> argMap);

    // Trims string with quotes
    static STRING trimQuotes(STRING s);

    // Compares the names on the LHS and RHS of the with clause
    // Stores the results of the with clause in the unordered map tempResults
    // Returns true if the clause can be satisfied and false otherwise
    static bool compareNames(unordered_map<STRING, STRING> firstArgMap, unordered_map<STRING, STRING> secondArgMap,
                             unordered_map<STRING, vector<int>>& tempResults);
    // Evaluates the with clause with attribute values of type NAME and one unknown argument
    // Returns a vector of values for the unknown argument represented by the argMap
    static vector<int> compareOneArgTypeName(STRING name, unordered_map<STRING, STRING> argMap);
    // Returns the name of the argument represented by the argMap at the statement number
    static STRING getName(unordered_map<STRING, STRING> argMap, StmtNum num);
};
