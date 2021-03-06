#include "WithEvaluator.h"

using namespace std;

WithEvaluator::WithEvaluator() {
}

bool WithEvaluator::evaluate(unordered_map<STRING, STRING> declarations, Clause clause,
                             unordered_map<STRING, vector<int>>& tempResults) {
    STRING firstArg = clause.getArgs().at(0);
    STRING secondArg = clause.getArgs().at(1);
    unordered_map<STRING, STRING> firstArgMap = getArgumentMap(firstArg, declarations);
    unordered_map<STRING, STRING> secondArgMap = getArgumentMap(secondArg, declarations);
    vector<int> results;
    bool isEqual;

    // both arguments are integer or both are name
    if (firstArgMap["argType"] == INTEGER_ && secondArgMap["argType"] == INTEGER_
            || firstArgMap["argType"] == NAME_ && secondArgMap["argType"] == NAME_) {
        return firstArgMap["arg"] == secondArgMap["arg"];
    }
    // both arguments are varName/procName/name
    if (firstArgMap["argType"] == NAME_ || secondArgMap["argType"] == NAME_
            || firstArgMap["attrName"] == "varName" || secondArgMap["attrName"] == "varName"
            || firstArgMap["attrName"] == "procName" || secondArgMap["attrName"] == "procName") {
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

unordered_map<STRING, STRING> WithEvaluator::getArgumentMap(STRING arg, unordered_map<STRING, STRING> declarations) { // [argType, attrName, synonym/integer/name]
    unordered_map<STRING, STRING> argMap;
    int posOfDot = arg.find('.');

    if (posOfDot == STRING::npos) { // no dot found
        argMap["argType"] = getArgType(arg, declarations);
        argMap["attrName"] = "";
        argMap["arg"] = arg;
    } else {
        argMap["attrName"] = arg.substr(posOfDot + 1, arg.length());
        argMap["arg"] = arg.substr(0, posOfDot);
        argMap["argType"] = getArgType(argMap["arg"], declarations);
    }
    return argMap;
}

STRING WithEvaluator::trimQuotes(STRING s) {
    return trim(s.substr(1, s.size() - 2));
}

void WithEvaluator::storeResults(vector<int> results, unordered_map<STRING, STRING> argMap,
                                 unordered_map<STRING, vector<int>>& tempResults) {
    vector<int> synonymValues;
    if (argMap["argType"] == NAME_ || argMap["argType"] == INTEGER_) {
        return; // don't need to store anything
    } else if (!results.empty()) {
        tempResults[argMap["arg"]] = results;
    }
}

vector<int> WithEvaluator::getValues(unordered_map<STRING, STRING> argMap) { // [argType, attrName, synonym/integer/name]
    if (argMap["argType"] == INTEGER_) {
        return vector<int>{stoi(argMap["arg"])};
    } else if (argMap["attrName"] == "procName") {
        if (argMap["argType"] == PROCEDURE_) {
            return PKB::procTable->getAllProcIDs();
        } else if (argMap["argType"] == CALL_) {
            return PKB::stmtTable->getAllCallStmtNums();
        }
    } else if (argMap["attrName"] == "varName") {
        if (argMap["argType"] == VARIABLE_) {
            return PKB::varTable->getAllVarIDs();
        } else if (argMap["argType"] == READ_) {
            return PKB::stmtTable->getAllReadStmtNums();
        } else if (argMap["argType"] == PRINT_) {
            return PKB::stmtTable->getAllPrintStmtNums();
        }
    } else if (argMap["attrName"] == "value") {
        return PKB::constTable->getAllConsts();
    } else if (argMap["attrName"] == "stmt#" || argMap["argType"] == PROGLINE_) {
        if (argMap["argType"] == STMT_ || argMap["argType"] == PROGLINE_) {
            return PKB::stmtTable->getAllStmtNums();
        } else if (argMap["argType"] == READ_) {
            return PKB::stmtTable->getAllReadStmtNums();
        } else if (argMap["argType"] == PRINT_) {
            return PKB::stmtTable->getAllPrintStmtNums();
        } else if (argMap["argType"] == CALL_) {
            return PKB::stmtTable->getAllCallStmtNums();
        } else if (argMap["argType"] == WHILE_) {
            return PKB::stmtTable->getAllWhileStmtNums();
        } else if (argMap["argType"] == IF_) {
            return PKB::stmtTable->getAllIfStmtNums();
        } else if (argMap["argType"] == ASSIGN_) {
            return PKB::stmtTable->getAllAssignStmtNums();
        }
    }
}

bool WithEvaluator::compareNames(unordered_map<STRING, STRING> firstArgMap, unordered_map<STRING, STRING> secondArgMap,
                                 unordered_map<STRING, vector<int>>& tempResults) {
    vector<int> results1;
    vector<int> results2;

    if (firstArgMap["argType"] == NAME_) {
        results1 = compareOneArgTypeName(trimQuotes(firstArgMap["arg"]), secondArgMap);
        if (!results1.empty()) {
            tempResults[secondArgMap["arg"]] = results1;
        }
        return !results1.empty();

    } else if (secondArgMap["argType"] == NAME_) {
        results2 = compareOneArgTypeName(trimQuotes(secondArgMap["arg"]), firstArgMap);
        if (!results2.empty()) {
            tempResults[firstArgMap["arg"]] = results2;
        }
        return !results2.empty();

    } else { // both are varName/procName
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

vector<int> WithEvaluator::compareOneArgTypeName(STRING name, unordered_map<STRING, STRING> argMap) {
    vector<int> results;
    STRING argType = argMap["argType"];
    if (argType == PROCEDURE_) {
        ProcID procId = PKB::procTable->getProcID(name);
        if (procId != -1) {
            results.push_back(procId);
        }
    } else if (argType == CALL_) {
        ProcID procId = PKB::procTable->getProcID(name);
        if (procId != -1) {
            unordered_set<StmtNum> stmts = PKB::calls->getStmtNumThatCallsCallee(procId);
            results.assign(stmts.begin(), stmts.end());
        }
    } else if (argType == VARIABLE_) {
        VarID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            results.push_back(varId);
        }
    } else if (argType == READ_) {
        VarID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            unordered_set<StmtNum> stmts = PKB::stmtTable->getStmtNumsOfReadWithVar(varId);
            results.assign(stmts.begin(), stmts.end());
        }
    } else { // argType == PRINT_
        VarID varId = PKB::varTable->getVarID(name);
        if (varId != -1) {
            unordered_set<StmtNum> stmts = PKB::stmtTable->getStmtNumsOfPrintWithVar(varId);
            results.assign(stmts.begin(), stmts.end());
        }
    }
    return results;
}

STRING WithEvaluator::getName(unordered_map<STRING, STRING> argMap, StmtNum num) {
    if (argMap["argType"] == NAME_) {
        return argMap["arg"];
    } else if (argMap["argType"] == PROCEDURE_) {
        return PKB::procTable->getProcName(num);
    } else if (argMap["argType"] == CALL_) {
        ProcID procCalled = PKB::calls->getCalleeInStmt(num);
        return PKB::procTable->getProcName(procCalled);
    } else if (argMap["argType"] == VARIABLE_) {
        return PKB::varTable->getVarName(num);
    } else if (argMap["argType"] == READ_) {
        VarID varRead = PKB::stmtTable->getReadVariableOfStmt(num);
        return PKB::varTable->getVarName(varRead);
    } else { // argMap["argType"] == PRINT_
        VarID varPrint = PKB::stmtTable->getPrintVariableOfStmt(num);
        return PKB::varTable->getVarName(varPrint);
    }
}

WithEvaluator::~WithEvaluator() {
}
