#include <iterator>
#include <set>

#include "WithEvaluator.h"
#include "QueryUtility.h"

using namespace std;

WithEvaluator::WithEvaluator() {
}

bool WithEvaluator::evaluate(unordered_map<string, string> declarations, Clause clause, 
	unordered_map<string, vector<int>>& tempResults) {
    string firstArg = clause.getArgs().at(0);
    string secondArg = clause.getArgs().at(1);
    unordered_map<string, string> firstArgMap = getArgumentMap(firstArg, declarations);
    unordered_map<string, string> secondArgMap = getArgumentMap(secondArg, declarations);
    vector<int> results;
    bool isEqual;

    // both arguments are integer or both are name
    if (firstArgMap["argType"] == INTEGER_ && secondArgMap["argType"] == INTEGER_ ||
        firstArgMap["argType"] == NAME_ && secondArgMap["argType"] == NAME_) {   
        return firstArgMap["arg"] == secondArgMap["arg"];
    }
    // both arguments are varName/procName/name
    if (firstArgMap["argType"] == NAME_ || secondArgMap["argType"] == NAME_ ||
        firstArgMap["attrName"] == "varName" || secondArgMap["attrName"] == "varName" ||
        firstArgMap["attrName"] == "procName" || secondArgMap["attrName"] == "procName") {
        return compareNames(firstArgMap, secondArgMap, tempResults);
    }
    // both arguments are stmt#/value/integer
    vector<int> allFirstArgValues = getValues(firstArgMap);
    vector<int> allSecondArgValues = getValues(secondArgMap);
    isEqual = intersectSingleSynonym(allFirstArgValues, allSecondArgValues, results);

    if (isEqual) {
        storeResults(results, firstArgMap, tempResults);
        storeResults(results, secondArgMap, tempResults);
    }
    return isEqual;
}

unordered_map<string, string> WithEvaluator::getArgumentMap(string arg, 
    unordered_map<string, string> declarations) { // [argType, attrName, synonym/integer/name]
    unordered_map<string, string> argMap;
    int posOfDot = arg.find(".");

    if (posOfDot == string::npos) { // no dot found
        argMap["argType"] = getArgType(arg, declarations);
        argMap["attrName"] = "";
        argMap["arg"] = arg;
    }
    else {
        argMap["attrName"] = arg.substr(posOfDot + 1, arg.length());
        argMap["arg"] = arg.substr(0, posOfDot);
        argMap["argType"] = getArgType(argMap["arg"], declarations);
    }
    return argMap;
}

// trim string with quotes
string WithEvaluator::trimQuotes(string s) {
    return trim(s.substr(1, s.size() - 2));
}

void WithEvaluator::storeResults(vector<int> results, unordered_map<string, string> argMap,
    unordered_map<string, vector<int>>& tempResults) {
    vector<int> synonymValues;
    if (argMap["argType"] == NAME_ || argMap["argType"] == INTEGER_) {
        return; // dunnid to store anything
    }
    else if (!results.empty()) {
        tempResults[argMap["arg"]] = results;
    }
}

vector<int> WithEvaluator::getValues(unordered_map<string, string> argMap) { //[argType, attrName, synonym/integer/name]
    if (argMap["argType"] == INTEGER_) {
        return vector<int>{stoi(argMap["arg"])};
    }
    else if (argMap["attrName"] == "procName") {
        if (argMap["argType"] == PROCEDURE_) {
            return PKB::procTable->getAllProcIDs();
        }
        else if (argMap["argType"] == CALL_) {
            return PKB::stmtTable->getAllCallStmtNums();
        }
    }
    else if (argMap["attrName"] == "varName") {
        if (argMap["argType"] == VARIABLE_) {
            return PKB::varTable->getAllVarIDs();
        } else if (argMap["argType"] == READ_) { 
            return PKB::stmtTable->getAllReadStmtNums();
        } else if (argMap["argType"] == PRINT_) {
            return PKB::stmtTable->getAllPrintStmtNums();
        }
    }
    else if (argMap["attrName"] == "value") {
        return PKB::constTable->getAllConsts();
    }
    else if (argMap["attrName"] == "stmt#" || argMap["argType"] == PROGLINE_) {
        if (argMap["argType"] == STMT_ || argMap["argType"] == PROGLINE_) {
            return PKB::stmtTable->getAllStmtNums();
        }
        else if (argMap["argType"] == READ_) {
            return PKB::stmtTable->getAllReadStmtNums();
        }
        else if (argMap["argType"] == PRINT_) {
            return PKB::stmtTable->getAllPrintStmtNums();
        }
        else if (argMap["argType"] == CALL_) {
            return PKB::stmtTable->getAllCallStmtNums();
        }
        else if (argMap["argType"] == WHILE_) {
            return PKB::stmtTable->getAllWhileStmtNums();
        }
        else if (argMap["argType"] == IF_) {
            return PKB::stmtTable->getAllIfStmtNums();
        }
        else if (argMap["argType"] == ASSIGN_) {
            return PKB::stmtTable->getAllAssignStmtNums();
        }
    }
}

bool WithEvaluator::compareNames(unordered_map<string, string> firstArgMap, unordered_map<string, string> secondArgMap,
    unordered_map<string, vector<int>>& tempResults) {
    vector<int> results1;
    vector<int> results2;
    if (firstArgMap["argType"] == NAME_) {
        results1 = compareOneArgTypeName(trimQuotes(firstArgMap["arg"]), secondArgMap);
        if (!results1.empty()) {
            tempResults[secondArgMap["arg"]] = results1;
        }
        return !results1.empty();
    }
    else if (secondArgMap["argType"] == NAME_) {
        results2 = compareOneArgTypeName(trimQuotes(secondArgMap["arg"]), firstArgMap);
        if (!results2.empty()) {
            tempResults[firstArgMap["arg"]] = results2;
        }
        return !results2.empty();
    }
    else { // both are varName/procName
        vector<int> allFirstArgValues = getValues(firstArgMap);
        vector<int> allSecondArgValues = getValues(secondArgMap);
        for (auto i = allFirstArgValues.begin(); i < allFirstArgValues.end(); i++) {
            for (auto j = allSecondArgValues.begin(); j < allSecondArgValues.end(); j++) {
                if (getName(firstArgMap, *i) == getName(secondArgMap, *j)) {
                    results1.push_back(*i);
                    results2.push_back(*j);
                }
            }
        }
        if (!results1.empty()) {
            tempResults[firstArgMap["arg"]] = results1;
            tempResults[secondArgMap["arg"]] = results2;
        }
        return !results1.empty();
    }
}

vector<int> WithEvaluator::compareOneArgTypeName(string name, unordered_map<string, string> argMap) {
    vector<int> results;
    string argType = argMap["argType"];
    if (argType == PROCEDURE_) {
        int procId = PKB::procTable->getProcID(name);
        if (procId != -1) {
            results.push_back(procId);
        }
    } else if (argType == CALL_) {
        ID procId = PKB::procTable->getProcID(name);
        if (procId != -1) {
            unordered_set<StmtNum> stmts = PKB::calls->getStmtNumThatCallsCallee(procId);
            results.assign(stmts.begin(), stmts.end());
        }
    } else if (argType == VARIABLE_) {
        ID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            results.push_back(varId);
        }
    } else if (argType == READ_) {
        ID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            unordered_set<StmtNum> stmts = PKB::stmtTable->getStmtNumsOfReadWithVar(varId);
            results.assign(stmts.begin(), stmts.end());
        }
    } else { // argType == PRINT_
        ID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            unordered_set<StmtNum> stmts = PKB::stmtTable->getStmtNumsOfPrintWithVar(varId);
            results.assign(stmts.begin(), stmts.end());
        }
    }
    /*vector<int> allArgValues = getValues(argMap);
    for (auto it = allArgValues.begin(); it < allArgValues.end(); it++) {
        if (getName(argMap, *it) == name) {
            results.push_back(*it);
        }
    }*/
    return results;
}

string WithEvaluator::getName(unordered_map<string, string> argMap, int num) {
    if (argMap["argType"] == NAME_) {
        return argMap["arg"];
    }
    else if (argMap["argType"] == PROCEDURE_) {
        return PKB::procTable->getProcName(num);
    }
    else if (argMap["argType"] == CALL_) {
        ID procCalled = PKB::calls->getCalleeInStmt(num);
        return PKB::procTable->getProcName(procCalled);
    }
    else if (argMap["argType"] == VARIABLE_) {
        return PKB::varTable->getVarName(num);
    }
    else if (argMap["argType"] == READ_) {
        ID varRead = PKB::stmtTable->getReadVariableOfStmt(num);
        return PKB::varTable->getVarName(varRead);
    }
    else if (argMap["argType"] == PRINT_) {
        ID varPrint = PKB::stmtTable->getPrintVariableOfStmt(num);
        return PKB::varTable->getVarName(varPrint);
    }
}

WithEvaluator::~WithEvaluator() {
}
