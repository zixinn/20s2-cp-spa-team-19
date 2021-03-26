#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() {

}

void QueryOptimizer::setIsGroup(bool isGroup) {
    this->isGroup = isGroup;
}

void QueryOptimizer::setIsOrderClauses(bool isOrderClauses) {
    this->isOrderClauses = isOrderClauses;
}

void QueryOptimizer::setIsOrderGroups(bool isOrderGroups) {
    this->isOrderGroups = isOrderGroups;
}

Query QueryOptimizer::optimize(Query query) {
    if (isGroup) {
        groupClauses(query);
    }
    if (isOrderClauses) {
        orderClauses(query);
    }
    if (isOrderGroups) {
        orderGroups(query);
    }
    return query;
}

void QueryOptimizer::groupClauses(Query& query) {
    // map synonym to index
    unordered_map<string, string> declarations = query.getDeclarations();
    int cnt = 0;
    unordered_map<string, int> synonymToIndex;
    for (auto& declaration : declarations) {
        synonymToIndex[declaration.first] = cnt;
        cnt++;
    }

    // populate adjacency list
    vector<unordered_set<int>> adj(declarations.size(), unordered_set<int>{});
    vector<Clause> clauses = query.getClauses().at(0);
    for (Clause clause : clauses) {
        unordered_set<string> synonymsInClause = clause.getSynonyms();
        unordered_set<int> indexOfSynonymsInClause = unordered_set<int>();
        for (string synonym : synonymsInClause) {
            indexOfSynonymsInClause.insert(synonymToIndex[synonym]);
        }
        for (int i : indexOfSynonymsInClause) {
            for (int j : indexOfSynonymsInClause) {
                if (i != j) {
                    adj.at(i).insert(j);
                }
            }
        }
    }

    // dfs to populate grouping map
    unordered_map<int, int> grouping = unordered_map<int, int>(); // map synonym index to group number
    int groupNum = 1;
    vector<bool> visited = vector<bool>(declarations.size(), false);
    for (int i = 0; i < declarations.size(); i++) {
        if (!visited.at(i)) {
            unordered_set<int> indexOfSynonymsInGroup = unordered_set<int>();
            dfs(i, adj, visited, indexOfSynonymsInGroup);
            for (int index : indexOfSynonymsInGroup) {
                grouping[index] = groupNum;
            }
            groupNum++;
        }
    }

    // group the clauses based on connected synonyms
    // first group contains clauses without synonyms
    // some groups may be empty, e.g. unused declarations
    vector<vector<Clause>> groups = vector<vector<Clause>>(groupNum, vector<Clause>{});
    vector<unordered_set<string>> synGroups = vector<unordered_set<string>>(groupNum, unordered_set<string>{});
    for (Clause clause : clauses) {
        if (clause.getSynonyms().empty()) { // no synonym in clause
            groups.at(0).push_back(clause); // insert at the front
        } else {
            string syn = *(clause.getSynonyms().begin());
            int idx = synonymToIndex[syn];
            int groupNumber = grouping[idx];
            groups.at(groupNumber).push_back(clause);
            for (string s : clause.getSynonyms()) {
                synGroups.at(groupNumber).insert(s);
            }
        }
    }
    query.setSynonyms(synGroups);
    query.setClauses(groups);
}

void QueryOptimizer::dfs(int source, vector<unordered_set<int>> adj, vector<bool>& visited, unordered_set<int>& indexOfSynonymsInGroup) {
    visited[source] = true;
    indexOfSynonymsInGroup.insert(source);
    for (int v : adj.at(source)) {
        if (!visited.at(v)) {
            dfs(v, adj, visited, indexOfSynonymsInGroup);
        }
    }
}

void QueryOptimizer::orderClauses(Query& query) {
    // sort by number of synonyms computed, then by number of known, then by size of table in PKB
    struct ClauseComparator {
        unordered_set<string> synonymsComputed;
        unordered_map<string, string> declarations;

        bool operator()(Clause& c1, Clause& c2) {
            int numSynComputedC1 = 0;
            for (string synonym : c1.getSynonyms()) {
                if (synonymsComputed.find(synonym) != synonymsComputed.end()) {
                    numSynComputedC1++;
                }
            }

            int numSynComputedC2 = 0;
            for (string synonym : c2.getSynonyms()) {
                if (synonymsComputed.find(synonym) != synonymsComputed.end()) {
                    numSynComputedC2++;
                }
            }

            if (numSynComputedC1 == numSynComputedC2) {
                if (c1.getNumOfKnown() == c2.getNumOfKnown()) {
                    return getSize(c1, declarations) < getSize(c2, declarations);
                } else {
                    return c1.getNumOfKnown() > c2.getNumOfKnown();
                }
            } else {
                return numSynComputedC1 > numSynComputedC2;
            }
        }
    };

    for (int i = 0; i < query.getClauses().size(); i++) { // for each group
        unordered_set<string> synonymsComputed;
        vector<Clause> oldClauses = query.getClauses().at(i);
        vector<Clause> newClauses;
        while (!oldClauses.empty()) {
            sort(oldClauses.begin(), oldClauses.end(), ClauseComparator({synonymsComputed, query.getDeclarations()}));
            newClauses.push_back(oldClauses.front());
            unordered_set<string> synonyms = oldClauses.front().getSynonyms();
            synonymsComputed.insert(synonyms.begin(), synonyms.end());
            oldClauses.erase(oldClauses.begin());
        }
        query.setClausesAtIdx(newClauses, i);
    }
}

void QueryOptimizer::orderGroups(Query& query) {
    vector<vector<Clause>> clauseGroups = query.getClauses();
    vector<unordered_set<string>> synonymGroups = query.getSynonyms();
    vector<vector<Clause>> orderedGroups;
    vector<vector<Clause>> groupsWithSynSelect; // groups with synonym in select clause
    vector<string> toSelect = query.getToSelect();
    int groupNum = -1;
    for (vector<Clause> group : clauseGroups) {
        groupNum++;
        if (group.empty()) {
            continue;
        }
        unordered_set<string> synonyms = synonymGroups.at(groupNum);
        if (synonyms.empty() || (toSelect.size() == 1 && toSelect.at(0) == "BOOLEAN")) {
            orderedGroups.push_back(group);
            continue;
        }
        bool flag = true;
        for (string s : toSelect) {
            int pos = s.find('.');
            string synonym = s;
            if (pos != string::npos) {
                synonym = s.substr(0, pos);
            }
            if (synonyms.find(synonym) != synonyms.end()) { // synonym to select in group
                groupsWithSynSelect.push_back(group);
                flag = false;
                break;
            }
        }
        if (flag) {
            orderedGroups.push_back(group);
        }
    }
    orderedGroups.insert(orderedGroups.end(), groupsWithSynSelect.begin(), groupsWithSynSelect.end());    
    query.setClauses(orderedGroups);
}

QueryOptimizer::~QueryOptimizer() {

}
