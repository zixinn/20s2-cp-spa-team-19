#include "QueryPreprocessor.h"

QueryPreprocessor::QueryPreprocessor() {
    designEntities = { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, VARIABLE_, CONSTANT_, PROCEDURE_, PROGLINE_ };

    validSuchThatArgType["Follows"] = { { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                        { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Follows*"] = { { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                         { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Parent"] = { { STMT_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                       { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Parent*"] = { { STMT_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                        { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Uses"] = { { STMT_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, NAME_ },
                                     { VARIABLE_, NAME_, UNDERSCORE_ } };
    validSuchThatArgType["Modifies"] = { { STMT_, READ_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, NAME_ },
                                         { VARIABLE_, NAME_, UNDERSCORE_ } };
    validSuchThatArgType["Calls"] = { { PROCEDURE_, NAME_, UNDERSCORE_ },
                                         { PROCEDURE_, NAME_, UNDERSCORE_ } };
    validSuchThatArgType["Calls*"] = { { PROCEDURE_, NAME_, UNDERSCORE_ },
                                      { PROCEDURE_, NAME_, UNDERSCORE_ } };
    validSuchThatArgType["Next"] = { { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                       { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Next*"] = { { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                     { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Affects"] = { { STMT_, ASSIGN_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                     { STMT_, ASSIGN_, PROGLINE_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Affects*"] = { { STMT_, ASSIGN_, PROGLINE_, INTEGER_, UNDERSCORE_ },
                                        { STMT_, ASSIGN_, PROGLINE_, INTEGER_, UNDERSCORE_ } };

    validPatternArgType["assign"] = { { VARIABLE_, NAME_, UNDERSCORE_ },
                                      { UNDERSCORE_, NAME_, EXPRESSION_, EXPRESSIONWITHUNDERSCORE_ } };
    validPatternArgType["while"] = { { VARIABLE_, NAME_, UNDERSCORE_ },
                                     { UNDERSCORE_ } };
    validPatternArgType["if"] = { { VARIABLE_, NAME_, UNDERSCORE_ },
                                      { UNDERSCORE_ },
                                      { UNDERSCORE_ } };
}

Query QueryPreprocessor::process(string query) {
    this->declarations.clear();
    this->toSelect.clear();
    this->clauses.clear();
    this->isSyntacticallyValid = true;
    this->isSemanticallyValid = true;

    if (query.empty() || query.at(query.length() - 1) == ';') {
        this->isSyntacticallyValid = false;
    }

    vector<string> statements;
    if (this->isSyntacticallyValid) {
        statements = split(query, ";");
        if (statements[statements.size() - 1].find("Select ") != 0) {
            this->isSyntacticallyValid = false;
        }
    }

    for (int i = 0; i < statements.size() - 1 && this->isSyntacticallyValid; i++) {
        if (!regex_match(statements[i], regex("^.*\\s.*$"))) {
            this->isSyntacticallyValid = false;
            break;
        }
        int space = statements[i].find(' ');
        string designEntity = trim(statements[i].substr(0, space));
        string synonyms = trim(statements[i].substr(space + 1));
        parseDeclaration(designEntity, synonyms);
    }

    if (this->isSyntacticallyValid) {
        parseSelect(trim(statements[statements.size() - 1].substr(7)));
    }

    return Query(this->declarations, this->toSelect, this->clauses, this->isSyntacticallyValid, this->isSemanticallyValid);
}

void QueryPreprocessor::parseDeclaration(string designEntity, string synonyms) {
    if (!checkDesignEntity(designEntity) || synonyms.at(synonyms.length() - 1) == ',') {
        this->isSyntacticallyValid = false;
    } else {
        vector<string> synonymsVector = split(synonyms, ",");
        for (string synonym : synonymsVector) {
            if (!checkName(synonym)) {
                this->isSyntacticallyValid = false;
                break;
            }
            if (checkSynonymDeclared(synonym, this->declarations)) {
                this->isSemanticallyValid = false;
            }
            if (this->isSemanticallyValid) {
                this->declarations[synonym] = designEntity;
            }
        }
    }
}

bool QueryPreprocessor::checkDesignEntity(string designEntity) {
    return this->designEntities.find(designEntity) != this->designEntities.end();
}

void QueryPreprocessor::parseSelect(string select) {
    int suchThatPos = select.find(" such that ");
    int patternPos = select.find(" pattern ");
    int nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
    int minPos = nextPos;

    if (nextPos == -1) {
        parseToSelect(select);
    } else {
        parseToSelect(trim(select.substr(0, nextPos)));
        while (this->isSyntacticallyValid && nextPos != -1) {
            if (minPos == suchThatPos) {
                suchThatPos = select.find(" such that ", suchThatPos + 1);
                nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
                parseSuchThatClauses(trim(select.substr(minPos + 11, nextPos - minPos - 11)));
            } else if (minPos == patternPos) {
                patternPos = select.find(" pattern ", patternPos + 1);
                nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
                parsePatternClauses(trim(select.substr(minPos + 9, nextPos - minPos - 9)));
            }
            minPos = nextPos;
        }
    }
}

int QueryPreprocessor::getNextPos(vector<int> pos) {
    int next = INT_MAX;
    for (int po : pos) {
        if (po != -1 && po < next) {
            next = po;
        }
    }
    return next == INT_MAX ?  -1 : next;
}

void QueryPreprocessor::parseToSelect(string synonym) {
    if (synonym == "BOOLEAN") {
        if (this->isSemanticallyValid) {
            this->toSelect.push_back(synonym);
        }

    } else if (regex_match(synonym, regex("^<.*>$"))) {
        string synonyms = trim(synonym.substr(1, synonym.length() - 2));
        if (synonyms.at(synonyms.length() - 1) == ',') {
            this->isSyntacticallyValid = false;
        } else {
            vector<string> synonymsVector = split(synonyms, ",");
            for (string syn : synonymsVector) {
                if (!checkName(syn)) {
                    this->isSyntacticallyValid = false;
                    break;
                } else if (!checkSynonymDeclared(syn, this->declarations)) {
                    this->isSemanticallyValid = false;
                }

                if (this->isSyntacticallyValid && this->isSemanticallyValid) {
                    this->toSelect.push_back(syn);
                }
            }
        }

    } else {
        if (!checkName(synonym)) {
            this->isSyntacticallyValid = false;
        } else if (!checkSynonymDeclared(synonym, this->declarations)) {
            this->isSemanticallyValid = false;
        }

        if (this->isSyntacticallyValid && this->isSemanticallyValid) {
            this->toSelect.push_back(synonym);
        }
    }
}

void QueryPreprocessor::parseSuchThatClauses(string clauses) {
    int andPos = -5;
    int nextPos = clauses.find(" and ");

    while (this->isSyntacticallyValid && nextPos != -1) {
        parseSuchThatClause(trim(clauses.substr(andPos + 5, nextPos - andPos - 5)));
        andPos = nextPos;
        nextPos = clauses.find(" and ", nextPos + 1);
    }

    if (this->isSyntacticallyValid) {
        parseSuchThatClause(trim(clauses.substr(andPos + 5, clauses.length() - andPos - 5)));
    }
}

void QueryPreprocessor::parseSuchThatClause(string clause) {
    if (!regex_match(clause, regex("^.*\\(.*,.*\\)$"))) {
        this->isSyntacticallyValid = false;
    } else {
        int left = clause.find('(');
        int comma = clause.find(',');
        int right = clause.length() - 1;

        string rel = trim(clause.substr(0, left));
        string firstArg = trim(clause.substr(left + 1, comma - left - 1));
        string secondArg = trim(clause.substr(comma + 1, right - comma - 1));
        checkSuchThatClause(rel, { firstArg, secondArg });
    }
}

void QueryPreprocessor::checkSuchThatClause(string rel, vector<string> args) {
    if (this->validSuchThatArgType.find(rel) == this->validSuchThatArgType.end()) {
        this->isSyntacticallyValid = false;
    } else {
        vector<unordered_set<string>> validArgType = this->validSuchThatArgType.find(rel)->second;
        for (int i = 0; i < validArgType.size(); i++) {
            string argType = getArgType(args[i], this->declarations);
            if (validArgType[i].find(argType) != validArgType[i].end()) {
                continue;
            }
            if (checkName(args[i])) {
                this->isSemanticallyValid = false;
            } else {
                this->isSyntacticallyValid = false;
                break;
            }
        }
    }

    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
        this->clauses.push_back(Clause(rel, args));
    }
}

void QueryPreprocessor::parsePatternClauses(string clauses) {
    int andPos = -5;
    int nextPos = clauses.find(" and ");

    while (this->isSyntacticallyValid && nextPos != -1) {
        parsePatternClause(trim(clauses.substr(andPos + 5, nextPos - andPos - 5)));
        andPos = nextPos;
        nextPos = clauses.find(" and ", nextPos + 1);
    }

    if (this->isSyntacticallyValid) {
        parsePatternClause(trim(clauses.substr(andPos + 5, clauses.length() - andPos - 5)));
    }
}

void QueryPreprocessor::parsePatternClause(string clause) {
    if (!regex_match(clause, regex("^.*\\(.*,.*\\)$")) && !regex_match(clause, regex("^.*\\(.*,.*,.*\\)$"))) {
        this->isSyntacticallyValid = false;
    } else {
        int left = clause.find('(');
        int comma = clause.find(',');
        int comma2 = clause.find(',', comma + 1);
        int right = clause.length() - 1;

        string syn = trim(clause.substr(0, left));
        string firstArg = trim(clause.substr(left + 1, comma - left - 1));

        if (comma2 == string::npos) { // assign, while
            string secondArg = trim(clause.substr(comma + 1, right - comma - 1));
            checkPatternClause(syn, { firstArg, secondArg });
        } else { // if
            string secondArg = trim(clause.substr(comma + 1, comma2 - comma - 1));
            string thirdArg = trim(clause.substr(comma2 + 1, right - comma2 - 1));
            checkPatternClause(syn, { firstArg, secondArg, thirdArg });
        }
    }
}

void QueryPreprocessor::checkPatternClause(string syn, vector<string> args) {
    string synArgType = getArgType(syn, this->declarations);
    if (synArgType != ASSIGN_ && synArgType != WHILE_ && synArgType != IF_) {
        this->isSyntacticallyValid = false;
    } else {
        vector<unordered_set<string>> validArgType = this->validPatternArgType.find(synArgType)->second;
        if (args.size() != validArgType.size()) {
            this->isSyntacticallyValid = false;
        }

        for (int i = 0; i < validArgType.size() && this->isSyntacticallyValid; i++) {
            string argType = getArgType(args[i], this->declarations);
            if (validArgType[i].find(argType) != validArgType[i].end()) {
                continue;
            }
            if (i == 0 && checkName(args[i])) {
                this->isSemanticallyValid = false;
            } else {
                this->isSyntacticallyValid = false;
                return;
            }
        }
    }

    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
        this->clauses.push_back(Clause(syn, args));
    }
}

QueryPreprocessor::~QueryPreprocessor() {

}
