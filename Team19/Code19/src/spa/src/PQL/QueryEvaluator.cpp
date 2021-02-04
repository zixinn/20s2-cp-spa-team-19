#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {

}

// Returns stmtNum of all stmt with type synonymType or ID of all proc/var/const with type synonymType
vector<int> QueryEvaluator::selectAll(string synonymType) {
    vector<int> res;
    if (synonymType == PROCEDURE_) {
        int size = PKB::procTable->getSize();
        for (int i = 0; i < size; i++) {
            res.push_back(i);
        }
        return res;
    } else if (synonymType == STMT_) {
        return PKB::stmtTable->getAllStmtNums();
    } else if (synonymType == READ_) {
        return PKB::stmtTable->getAllReadStmtNums();
    } else if (synonymType == PRINT_) {
        return PKB::stmtTable->getAllPrintStmtNums();
    } else if (synonymType == ASSIGN_) {
        return PKB::stmtTable->getAllAssignStmtNums();
    } else if (synonymType == CALL_) {
        return PKB::stmtTable->getAllCallStmtNums();
    } else if (synonymType == WHILE_) {
        return PKB::stmtTable->getAllWhileStmtNums();
    } else if (synonymType == IF_) {
        return PKB::stmtTable->getAllIfStmtNums();
    } else if (synonymType == VARIABLE_) {
        int size = PKB::varTable->getSize();
        for (int i = 0; i < size; i++) {
            res.push_back(i);
        }
        return res;
    } else if (synonymType == CONSTANT_) {
        return PKB::constTable->getAllConsts();
    }
    return res;
}

// Evaluates the query and returns a list containing the answers to the query
list<string> QueryEvaluator::evaluate(Query query) {
    list<string> emptyList;

    if (!query.getIsValid()) {
        return emptyList;
    }

    this->declarations = query.getDeclarations();
    this->toSelect = query.getToSelect();
    this->clauses = query.getClauses();

    for (Clause clause : this->clauses) {
        unordered_map<string, vector<int>> tempResults;
        if (!evaluateClause(clause, tempResults)) {
            return emptyList;
        }
        if (!tempResults.empty()) {
            join(tempResults);
        }
    }

    return evaluateSynonymToSelect(this->toSelect);
}

// Evaluates the clause and stores the results for the clause in tempResults
// Returns false if the clause is false or cannot be satisfied
bool QueryEvaluator::evaluateClause(Clause clause, unordered_map<string, vector<int>>& tempResults) {
//    string rel = clause.getRel();
//    if (rel == "Follows") {
//        FollowsEvaluator evaluator = FollowsEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else if (rel == "Follows*") {
//        FollowsTEvaluator evaluator = FollowsTEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else if (rel == "Parent") {
//        ParentEvaluator evaluator = ParentEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else if (rel == "Parent*") {
//        ParentTEvaluator evaluator = ParentTEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else if (rel == "Uses") {
//        UsesEvaluator evaluator = UsesEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else if (rel == "Modifies") {
//        ModifiesEvaluator evaluator = ModifiesEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    } else { // pattern
//        PatternEvaluator evaluator = PatternEvaluator();
//        return evaluator.evaluate(this->declarations, clause, tempResults);
//    }
    return true;
}

// Joins table and results based on common synonyms (if any) and assigns the newResults to results
void QueryEvaluator::join(unordered_map<string, vector<int>> table) {
    if (results.empty()) {
        results = table;
        return;
    }

    unordered_set<string> allSynonyms;
    for (pair<string, vector<int>> col : results) {
        allSynonyms.insert(col.first);
    }
    for (pair<string, vector<int>> col : table) {
        allSynonyms.insert(col.first);
    }
    unordered_map<string, vector<int>> newResults;
    for (string synonym : allSynonyms) {
        vector<int> v;
        newResults.insert({ synonym, v });
    }

    vector<string> commonSynonyms;
    for (pair<string, vector<int>> col : table) {
        if (results.find(col.first) != results.end()) {
            commonSynonyms.push_back(col.first);
        }
    }

    int resultsNumRows = results.begin()->second.size();
    int tableNumRows = table.begin()->second.size();
    // table will have at most 2 cols
    if (commonSynonyms.empty()) {
        for (int i = 0; i < resultsNumRows; i++) {
            for (int j = 0; j < tableNumRows; j++) {
                // insert row i in results into newResults
                for (pair<string, vector<int>> col : results) {
                    newResults[col.first].push_back(col.second.at(i));
                }
                // insert row j in table into newResults
                for (pair<string, vector<int>> col : table) {
                    newResults[col.first].push_back(col.second.at(j));
                }
            }
        }

    } else if (commonSynonyms.size() == 1) {
        string commonSynonym = commonSynonyms.at(0);
        if (table.size() == 1) {
            unordered_set<string> set;
            for (int i = 0; i < tableNumRows; i++) {
                int val = table[commonSynonym].at(i); // value of commonSynonym in table at row i
                string s = to_string(val);
                set.insert(s);
            }
            for (int i = 0; i < resultsNumRows; i++) {
                int val = results[commonSynonym].at(i); // value of commonSynonym in results at row i
                string s = to_string(val);
                if (set.find(s) != set.end()) {
                    // insert row i in results into newResults
                    for (pair<string, vector<int>> col : results) {
                        newResults[col.first].push_back(col.second.at(i));
                    }
                }
            }
//            for (int i = 0; i < resultsNumRows; i++) {
//                int val = results[commonSynonym].at(i); // value of commonSynonym in results at row i
//                vector<int> v = table[commonSynonym];
//                if (find(v.begin(), v.end(), val) != v.end()) {
//                    // insert row i in results into newResults
//                    for (pair<string, vector<int>> col : results) {
//                        newResults[col.first].push_back(col.second.at(i));
//                    }
//                }
//            }

        } else if (table.size() == 2) {
            string otherSynonym;
            for (pair<string, vector<int>> col : results) {
                if (col.first != commonSynonym) {
                    otherSynonym = col.first;
                    break;
                }
            }
            unordered_map<string, vector<int>> map; // map commonSynonym to index in table
            for (int i = 0; i < tableNumRows; i++) {
                int val = table[commonSynonym].at(i); // value of commonSynonym in table at row i
                string s = to_string(val);
                if (map.find(s) != map.end()) {
                    map.find(s)->second.push_back(i);
                } else {
                    map.insert({s, {i}});
                }
            }
            for (int i = 0; i < resultsNumRows; i++) {
                int val = results[commonSynonym].at(i); // value of commonSynonym in results at row i
                string s = to_string(val);
                auto it = map.find(s);
                if (it != map.end()) {
                    int size = it->second.size();
                    for (int j = 0; j < size; j++) {
                        int k = it->second.at(j);
                        // insert row i in results into newResults
                        for (pair<string, vector<int>> col : results) {
                            newResults[col.first].push_back(col.second.at(i));
                        }
                        // insert row k in table into newResults
                        for (pair<string, vector<int>> col : table) {
                            if (col.first != commonSynonym) {
                                newResults[col.first].push_back(col.second.at(k));
                            }
                        }
                    }
                }
            }
//            for (int i = 0; i < resultsNumRows; i++) {
//                int val = results[commonSynonym].at(i); // value of commonSynonym in results at row i
//                for (int j = 0; j < tableNumRows; j++) {
//                    if (table[commonSynonym].at(j) == val) {
//                        // insert row i in results into newResults
//                        for (pair<string, vector<int>> col : results) {
//                            newResults[col.first].push_back(col.second.at(i));
//                        }
//                        // insert row j in table into newResults
//                        for (pair<string, vector<int>> col : table) {
//                            if (col.first != commonSynonym) {
//                                newResults[col.first].push_back(col.second.at(j));
//                            }
//                        }
//                    }
//                }
//            }
        }

    } else if (commonSynonyms.size() == 2) {
        string commonSynonym1 = commonSynonyms.at(0);
        string commonSynonym2 = commonSynonyms.at(1);
        unordered_set<string> set;
        for (int i = 0; i < tableNumRows; i++) {
            int val1 = table[commonSynonym1].at(i); // value of commonSynonym1 in table at row i
            int val2 = table[commonSynonym2].at(i); // value of commonSynonym2 in table at row i
            string s = to_string(val1) + " " + to_string(val2);
            set.insert(s);
        }
        for (int i = 0; i < resultsNumRows; i++) {
            int val1 = results[commonSynonym1].at(i); // value of commonSynonym1 in results at row i
            int val2 = results[commonSynonym2].at(i); // value of commonSynonym2 in results at row i
            string s = to_string(val1) + " " + to_string(val2);
            if (set.find(s) != set.end()) {
                // insert row i in results into newResults
                for (pair<string, vector<int>> col : results) {
                    newResults[col.first].push_back(col.second.at(i));
                }
            }
        }
//        string commonSynonym1 = commonSynonyms.at(0);
//        string commonSynonym2 = commonSynonyms.at(1);
//        for (int i = 0; i < resultsNumRows; i++) {
//            int val1 = results[commonSynonym1].at(i); // value of commonSynonym1 in results at row i
//            int val2 = results[commonSynonym2].at(i); // value of commonSynonym2 in results at row i
//            for (int j = 0; j < tableNumRows; j++) {
//                if (table[commonSynonym1].at(j) == val1 && table[commonSynonym2].at(j) == val2) {
//                    // insert row i in results into newResults
//                    for (pair<string, vector<int>> col : results) {
//                        newResults[col.first].push_back(col.second.at(i));
//                    }
//                    break;
//                }
//            }
//        }
    }

    results = newResults;
}

// Evaluates the synonym to select using the results table and returns a list containing the answers
list<string> QueryEvaluator::evaluateSynonymToSelect(string toSelect) {
    if (results.find(toSelect) == results.end()) {
        unordered_map<string, vector<int>> tempResults = {{toSelect, selectAll(this->declarations[toSelect])}};
        join(tempResults);
    }

    unordered_set<string> set;
    string toSelectType = this->declarations[toSelect];
    int numRows = results.begin()->second.size();
    for (int i = 0; i < numRows; i++) {
        int val = results[toSelect].at(i);
        if (toSelectType == PROCEDURE_) {
            set.insert(PKB::procTable->getProcName(val));
        } else if (toSelectType == VARIABLE_) {
            set.insert(PKB::varTable->getVarName(val));
        } else {
            set.insert(to_string(val));
        }
    }

    list<string> res(set.begin(), set.end());
    return res;
}

QueryEvaluator::~QueryEvaluator() {

}
