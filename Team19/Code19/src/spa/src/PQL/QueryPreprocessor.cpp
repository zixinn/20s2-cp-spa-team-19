#include "QueryPreprocessor.h"

QueryPreprocessor::QueryPreprocessor() {
    designEntities = { STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, VARIABLE_, CONSTANT_, PROCEDURE_, PROGLINE_ };
    attrNames = { "procName", "varName", "value", "stmt#" };

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

    attrMap[PROCEDURE_] = { "procName" };
    attrMap[CALL_] = { "procName", "stmt#" };
    attrMap[VARIABLE_] = { "varName" };
    attrMap[READ_] = { "varName", "stmt#" };
    attrMap[PRINT_] = { "varName", "stmt#" };
    attrMap[CONSTANT_] = { "value" };
    attrMap[STMT_] = { "stmt#" };
    attrMap[WHILE_] = { "stmt#" };
    attrMap[IF_] = { "stmt#" };
    attrMap[ASSIGN_] = { "stmt#" };
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

    return Query(this->declarations, this->toSelect, { this->clauses }, this->isSyntacticallyValid, this->isSemanticallyValid);
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
    int withPos = select.find(" with ");
    int nextPos = getNextPos(vector<int>{suchThatPos, patternPos, withPos});
    int minPos = nextPos;

    if (nextPos == -1) {
        parseToSelect(select);
    } else {
        parseToSelect(trim(select.substr(0, nextPos)));
        while (this->isSyntacticallyValid && nextPos != -1) {
            if (minPos == suchThatPos) {
                suchThatPos = select.find(" such that ", suchThatPos + 1);
                nextPos = getNextPos(vector<int>{suchThatPos, patternPos, withPos});
                parseSuchThatClauses(trim(select.substr(minPos + 11, nextPos - minPos - 11)));
            } else if (minPos == patternPos) {
                patternPos = select.find(" pattern ", patternPos + 1);
                nextPos = getNextPos(vector<int>{suchThatPos, patternPos, withPos});
                parsePatternClauses(trim(select.substr(minPos + 9, nextPos - minPos - 9)));
            } else if (minPos == withPos) {
                withPos = select.find(" with ", withPos + 1);
                nextPos = getNextPos(vector<int>{suchThatPos, patternPos, withPos});
                parseWithClauses(trim(select.substr(minPos + 6, nextPos - minPos - 6)));
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
    return next == INT_MAX ? -1 : next;
}

void QueryPreprocessor::parseToSelect(string resultCl) {
    if (resultCl == "BOOLEAN") {
        if (this->isSemanticallyValid) {
            this->toSelect.push_back(resultCl);
        }
        if (checkSynonymDeclared(resultCl, this->declarations)) {
            this->isSemanticallyValid = false;
        }

    } else if (regex_match(resultCl, regex("^<.*>$"))) { // tuples
        string synonyms = trim(resultCl.substr(1, resultCl.length() - 2));
        if (synonyms.at(synonyms.length() - 1) == ',') {
            this->isSyntacticallyValid = false;
        } else {
            vector<string> synonymsVector = split(synonyms, ",");
            for (string elem : synonymsVector) {
                int pos = elem.find('.');
                if (pos == string::npos) { // synonym
                    if (!checkName(elem)) {
                        this->isSyntacticallyValid = false;
                        break;
                    } else if (!checkSynonymDeclared(elem, this->declarations)) {
                        this->isSemanticallyValid = false;
                    }
                    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
                        this->toSelect.push_back(elem);
                    }

                } else { // attrRef
                    string synonym = trim(elem.substr(0, pos));
                    string attrName = trim(elem.substr(pos + 1));
                    checkAttrRef(synonym, attrName);
                    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
                        this->toSelect.push_back(synonym.append(".").append(attrName));
                    }
                }
            }
        }

    } else { // elem
        int pos = resultCl.find('.');
        if (pos == string::npos) { // synonym
            if (!checkName(resultCl)) {
                this->isSyntacticallyValid = false;
            } else if (!checkSynonymDeclared(resultCl, this->declarations)) {
                this->isSemanticallyValid = false;
            }
            if (this->isSyntacticallyValid && this->isSemanticallyValid) {
                this->toSelect.push_back(resultCl);
            }

        } else { //attrRef
            string synonym = trim(resultCl.substr(0, pos));
            string attrName = trim(resultCl.substr(pos + 1));
            checkAttrRef(synonym, attrName);

            if (this->isSyntacticallyValid && this->isSemanticallyValid) {
                this->toSelect.push_back(synonym.append(".").append(attrName));
            }
        }
    }
}

void QueryPreprocessor::checkAttrRef(string synonym, string attrName) {
    if (!checkName(synonym) || attrNames.find(attrName) == attrNames.end()) {
        this->isSyntacticallyValid = false;
    } else {
        auto it = this->attrMap.find(getArgType(synonym, this->declarations));
        if (it == this->attrMap.end() || it->second.find(attrName) == it->second.end()) {
            this->isSemanticallyValid = false;
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
    unordered_set<string> synonyms = unordered_set<string>();
    int numOfKnown = 0;
    if (this->validSuchThatArgType.find(rel) == this->validSuchThatArgType.end()) {
        this->isSyntacticallyValid = false;
    } else {
        vector<unordered_set<string>> validArgType = this->validSuchThatArgType.find(rel)->second;
        for (int i = 0; i < validArgType.size(); i++) {
            string argType = getArgType(args[i], this->declarations);
            if (validArgType[i].find(argType) != validArgType[i].end()) {
                if (argType == INTEGER_ || argType == NAME_) {
                    numOfKnown++;
                } else if (checkDesignEntity(argType)) {
                    synonyms.insert(args[i]);
                }
                continue;
            }
            if (argType == UNDERSCORE_ || checkName(args[i])) {
                this->isSemanticallyValid = false;
            } else {
                this->isSyntacticallyValid = false;
                break;
            }
        }
    }

    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
        this->clauses.push_back(Clause(rel, args, synonyms, numOfKnown));
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
    unordered_set<string> synonyms = unordered_set<string>();
    int numOfKnown = 0;
    string synArgType = getArgType(syn, this->declarations);
    if (synArgType != ASSIGN_ && synArgType != WHILE_ && synArgType != IF_) {
        this->isSyntacticallyValid = false;
    } else {
        synonyms.insert(syn);
        vector<unordered_set<string>> validArgType = this->validPatternArgType.find(synArgType)->second;
        if (args.size() != validArgType.size()) {
            this->isSyntacticallyValid = false;
        }

        for (int i = 0; i < validArgType.size() && this->isSyntacticallyValid; i++) {
            string argType = getArgType(args[i], this->declarations);
            if (validArgType[i].find(argType) != validArgType[i].end()) {
                if (argType == NAME_ || argType == EXPRESSION_ || argType == EXPRESSIONWITHUNDERSCORE_) {
                    numOfKnown++;
                } else if (checkDesignEntity(argType)) {
                    synonyms.insert(args[i]);
                }
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
        this->clauses.push_back(Clause(syn, args, synonyms, numOfKnown));
    }
}

void QueryPreprocessor::parseWithClauses(string clauses) {
    int andPos = -5;
    int nextPos = clauses.find(" and ");

    while (this->isSyntacticallyValid && nextPos != -1) {
        parseWithClause(trim(clauses.substr(andPos + 5, nextPos - andPos - 5)));
        andPos = nextPos;
        nextPos = clauses.find(" and ", nextPos + 1);
    }

    if (this->isSyntacticallyValid) {
        parseWithClause(trim(clauses.substr(andPos + 5, clauses.length() - andPos - 5)));
    }
}

void QueryPreprocessor::parseWithClause(string clause) {
    int pos = clause.find('=');
    if (pos == string::npos) {
        this->isSyntacticallyValid = false;
    } else {
        string left = trim(clause.substr(0, pos));
        string right = trim(clause.substr(pos + 1));
        checkWithClause(left, right);
    }
}

void QueryPreprocessor::checkWithClause(string left, string right) {
    unordered_set<string> synonyms = unordered_set<string>();
    int numOfKnown = 0;
    string argTypeLeft;
    string argTypeRight;
    string leftType = checkRef(left, synonyms, numOfKnown, argTypeLeft);
    string rightType = checkRef(right, synonyms, numOfKnown, argTypeRight);
    if (this->isSyntacticallyValid && leftType != rightType) {
        this->isSemanticallyValid = false;
    }

    if (this->isSyntacticallyValid && this->isSemanticallyValid) {
        if (argTypeLeft == INTEGER_ || argTypeLeft == NAME_) {
            this->clauses.push_back(Clause("", {right, left}, synonyms, numOfKnown));
        } else {
            this->clauses.push_back(Clause("", {left, right}, synonyms, numOfKnown));
        }
    }
}

string QueryPreprocessor::checkRef(string &ref, unordered_set<string>& synonyms, int& numOfKnown, string& argType) {
    string type;
    int pos = ref.find('.');
    if (pos == string::npos) { // NAME_ | INTEGER_ | synonym
        argType = getArgType(ref, this->declarations);
        if (argType == NAME_) {
            type = NAME_;
            numOfKnown++;
        } else if (argType == INTEGER_) {
            type = INTEGER_;
            numOfKnown++;
        } else if (argType == PROGLINE_) {
            type = INTEGER_;
            synonyms.insert(ref);
        } else {
            if (checkName(ref)) { // synonym
                this->isSemanticallyValid = false;
            } else {
                this->isSyntacticallyValid = false;
            }
            return type;
        }
    } else { // attrRef
        string synonym = trim(ref.substr(0, pos));
        string attrName = trim(ref.substr(pos + 1));
        checkAttrRef(synonym, attrName);
        if (!this->isSyntacticallyValid) {
            return type;
        }
        if (this->isSyntacticallyValid) {
            if (attrName == "value" || attrName == "stmt#") {
                type = INTEGER_;
            } else {
                type = NAME_;
            }
            synonyms.insert(synonym);
        }
        ref = synonym.append(".").append(attrName);
    }
    return type;
}

QueryPreprocessor::~QueryPreprocessor() {

}
