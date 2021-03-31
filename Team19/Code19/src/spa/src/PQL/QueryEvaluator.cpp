#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {

}

list<string> QueryEvaluator::evaluate(Query query) {
    this->declarations = query.getDeclarations();
    this->toSelect = query.getToSelect();
    this->synonymsToSelect = unordered_set<string>();
    for (string synonym : toSelect) {
        string syn = synonym.substr(0, synonym.find('.'));
        this->synonymsToSelect.insert(syn);
    }
    this->clauses = query.getClauses();
    this->results.clear();

    list<string> emptyList;

    if (!query.getIsSyntacticallyValid()) {
        return emptyList;
    }
    if (!query.getIsSemanticallyValid()) {
        if (toSelect.size() == 1 && toSelect.at(0) == "BOOLEAN") {
            return list<string> {"FALSE"};
        }
        return emptyList;
    }

    for (vector<Clause> clauses : this->clauses) {
        unordered_map<string, vector<int>> groupResults;
        for (int j = 0; j < clauses.size(); j++) {
            Clause clause = clauses.at(j);
            unordered_map<string, vector<int>> tempResults;
            if (!evaluateClause(clause, tempResults)) { // clause returns false
                if (toSelect.size() == 1 && toSelect.at(0) == "BOOLEAN") {
                    return list<string> {"FALSE"};
                }
                return emptyList;
            }
            if (!tempResults.empty()) {
                unordered_set<string> toProject = this->synonymsToSelect; // add synonyms in result-cl
                for (int k = j + 1; k < clauses.size(); k++) { // add synonyms in remaining clauses
                    unordered_set<string> synonyms = clauses.at(k).getSynonyms();
                    toProject.insert(synonyms.begin(), synonyms.end());
                }
                vector<string> commonSynonyms;
                for (pair<string, vector<int>> col : tempResults) {
                    if (groupResults.find(col.first) != groupResults.end()) {
                        commonSynonyms.push_back(col.first);
                    }
                }
                unordered_set<string> toProjectWithCommonSynonyms;
                toProjectWithCommonSynonyms.insert(toProject.begin(), toProject.end());
                toProjectWithCommonSynonyms.insert(commonSynonyms.begin(), commonSynonyms.end());
                project(toProjectWithCommonSynonyms, tempResults);

                if (!tempResults.empty()) {
                    join(tempResults, groupResults, commonSynonyms);
                    if (groupResults.empty() || groupResults.begin()->second.empty()) {
                        if (toSelect.size() == 1 && toSelect.at(0) == "BOOLEAN") {
                            return list<string> {"FALSE"};
                        }
                        return emptyList;
                    }
                    project(toProject, groupResults);
                }
            }
        }
        project(this->synonymsToSelect, groupResults);
        if (!groupResults.empty()) {
            join(groupResults, results, vector<string>());
        }
    }

    return evaluateSynonymToSelect(this->toSelect);
}

bool QueryEvaluator::evaluateClause(Clause clause, unordered_map<string, vector<int>>& tempResults) {
    string rel = clause.getRel();
    if (rel == "Follows") {
        return FollowsEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Follows*") {
        return FollowsTEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Parent") {
        return ParentEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Parent*") {
        return ParentTEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Uses") {
        return UsesEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Modifies") {
        return ModifiesEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Calls") {
        return CallsEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Calls*") {
        return CallsTEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Next") {
        return NextEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Next*") {
        return NextTEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Affects") {
        return AffectsEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "Affects*") {
        return AffectsTEvaluator::evaluate(this->declarations, clause, tempResults);
    } else if (rel == "") { // with clause
        return WithEvaluator::evaluate(this->declarations, clause, tempResults);
    } else { // pattern clause
        return PatternEvaluator::evaluate(this->declarations, clause, tempResults);
    }
}

void QueryEvaluator::join(unordered_map<string, vector<int>> table, unordered_map<string, vector<int>>& results, vector<string> commonSynonyms) {
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

    int resultsNumRows = results.begin()->second.size();
    int tableNumRows = table.begin()->second.size();
    if (commonSynonyms.empty()) { // cross product
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

    // table will have at most 2 cols
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

        } else if (table.size() == 2) {
            unordered_map<string, vector<int>> map; // map commonSynonym to index in table
            for (int i = 0; i < tableNumRows; i++) {
                int val = table[commonSynonym].at(i); // value of commonSynonym in table at row i
                string s = to_string(val);
                if (map.find(s) != map.end()) {
                    map.find(s)->second.push_back(i);
                } else {
                    map.insert({ s, {i} });
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
    }

    results = newResults;
}

list<string> QueryEvaluator::evaluateSynonymToSelect(vector<string> toSelect) {
    if (toSelect.size() == 1 && toSelect.at(0) == "BOOLEAN") {
        return list<string> {"TRUE"};
    }

    for (string synonym : synonymsToSelect) {
        if (results.find(synonym) == results.end()) { // synonym not in results table
            unordered_map<string, vector<int>> tempResults = { {synonym, selectAll(this->declarations[synonym])} };
            join(tempResults, results, vector<string>());
        }
    }

    unordered_set<string> set;
    int numRows = results.begin()->second.size();
    for (int i = 0; i < numRows; i++) {
        string res = "";
        for (int j = 0; j < toSelect.size(); j++) {
            string synonym, attribute;
            int pos = toSelect[j].find('.');
            if (pos == string::npos) {
                synonym = toSelect[j];
            } else {
                synonym = toSelect[j].substr(0, pos);
                attribute = toSelect[j].substr(pos + 1);
            }

            string toSelectType = this->declarations[synonym];
            int val = results[synonym].at(i);
            if (toSelectType == CALL_ && attribute == "procName") {
                int procId = PKB::calls->getCalleeInStmt(val);
                res += PKB::procTable->getProcName(procId);
            } else if (toSelectType == READ_ && attribute == "varName") {
                int varId = PKB::stmtTable->getReadVariableOfStmt(val);
                res += PKB::varTable->getVarName(varId);
            } else if (toSelectType == PRINT_ && attribute == "varName") {
                int varId = PKB::stmtTable->getPrintVariableOfStmt(val);
                res += PKB::varTable->getVarName(varId);
            } else if (toSelectType == PROCEDURE_) {
                res += PKB::procTable->getProcName(val);
            } else if (toSelectType == VARIABLE_) {
                res += PKB::varTable->getVarName(val);
            } else {
                res += to_string(val);
            }
            if (j != toSelect.size() - 1) {
                res += " ";
            }
        }
        set.insert(res);
    }

    list<string> res(set.begin(), set.end());
    return res;
}

QueryEvaluator::~QueryEvaluator() {

}
