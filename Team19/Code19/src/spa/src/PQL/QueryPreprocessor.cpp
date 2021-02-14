#include "QueryPreprocessor.h"

QueryPreprocessor::QueryPreprocessor() {
    designEntities = { PROCEDURE_, STMTLST_, STMT_, READ_, PRINT_, ASSIGN_, CALL_, WHILE_, IF_, VARIABLE_, CONSTANT_ };
    validSuchThatArgType["Follows"] = { { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ },
                                        { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Follows*"] = { { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ },
                                         { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Parent"] = { { STMT_, WHILE_, IF_, INTEGER_, UNDERSCORE_ },
                                       { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Parent*"] = { { STMT_, WHILE_, IF_, INTEGER_, UNDERSCORE_ },
                                        { STMT_, READ_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, UNDERSCORE_ } };
    validSuchThatArgType["Uses"] = { { STMT_, PRINT_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, NAME_ },
                                     { VARIABLE_, NAME_, UNDERSCORE_ } };
    validSuchThatArgType["Modifies"] = { { STMT_, READ_, PROCEDURE_, ASSIGN_, CALL_, WHILE_, IF_, INTEGER_, NAME_ },
                                         { VARIABLE_, NAME_, UNDERSCORE_ } };
    validPatternArgType["assign"] = { { VARIABLE_, NAME_, UNDERSCORE_ },
                                      { UNDERSCORE_, NAME_, EXPRESSION_, EXPRESSIONWITHUNDERSCORE_ } };
}

Query QueryPreprocessor::process(string query) {
    this->declarations.clear();
    this->toSelect = "";
    this->clauses.clear();
    this->isValid = true;

    if (query.empty() || query.at(query.length() - 1) == ';') {
        this->isValid = false;
    }

    vector<string> statements;
    if (this->isValid) {
        statements = split(query, ";");
        if (statements[statements.size() - 1].find("Select ") != 0) {
            this->isValid = false;
        }
    }

    for (int i = 0; i < statements.size() - 1 && this->isValid; i++) {
        if (!regex_match(statements[i], regex("^.*\\s.*$"))) {
            this->isValid = false;
            break;
        }
        int space = statements[i].find(' ');
        string designEntity = trim(statements[i].substr(0, space));
        string synonyms = trim(statements[i].substr(space + 1));
        parseDeclaration(designEntity, synonyms);
    }

    if (this->isValid) {
        parseSelect(trim(statements[statements.size() - 1].substr(7)));
    }

    return Query(this->declarations, this->toSelect, this->clauses, this->isValid);
}

bool QueryPreprocessor::parseDeclaration(string designEntity, string synonyms) {
    if (!checkDesignEntity(designEntity)) {
        this->isValid = false;
        return false;
    }
    if (synonyms.at(synonyms.length() - 1) == ',') {
        return false;
    }
    vector<string> synonymsVector = split(synonyms, ",");
    for (string synonym : synonymsVector) {
        if (checkSynonymDeclared(synonym, this->declarations) || !checkName(synonym)) {
            this->isValid = false;
            return false;
        }
        this->declarations[synonym] = designEntity;
    }
    return true;
}

bool QueryPreprocessor::checkDesignEntity(string designEntity) {
    return this->designEntities.find(designEntity) != this->designEntities.end();
}

bool QueryPreprocessor::parseSelect(string select) {
    int suchThatPos = select.find(" such that ");
    int patternPos = select.find(" pattern ");
    int nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
    int minPos = nextPos;

    if (nextPos == -1) {
        return parseToSelect(select);
    }
    if (!parseToSelect(trim(select.substr(0, nextPos)))) {
        return false;
    }

    while (nextPos != -1) {
        if (minPos == suchThatPos) {
            suchThatPos = select.find(" such that ", suchThatPos + 1);
            nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
            if (!parseSuchThatClause(trim(select.substr(minPos + 11, nextPos - minPos - 11)))) {
                return false;
            }
        } else if (minPos == patternPos) {
            patternPos = select.find(" pattern ", patternPos + 1);
            nextPos = getNextPos(vector<int>{suchThatPos, patternPos});
            if (!parsePatternClause(trim(select.substr(minPos + 9, nextPos - minPos - 9)))) {
                return false;
            }
        }
        minPos = nextPos;
    }
    return true;
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

bool QueryPreprocessor::parseToSelect(string synonym) {
    if (!checkSynonymDeclared(synonym, this->declarations)) {
        this->isValid = false;
        return false;
    }
    this->toSelect = synonym;
    return true;
}

bool QueryPreprocessor::parseSuchThatClause(string clause) {
    if (!regex_match(clause, regex("^.*\\(.*,.*\\)$"))) {
        this->isValid = false;
        return false;
    }

    int left = clause.find('(');
    int comma = clause.find(',');
    int right = clause.find(')');

    string rel = trim(clause.substr(0, left));
    string firstArg = trim(clause.substr(left + 1, comma - left - 1));
    string secondArg = trim(clause.substr(comma + 1, right - comma - 1));

    if (!checkSuchThatClause(rel, { firstArg, secondArg })) {
        this->isValid = false;
        return false;
    }

    this->clauses.push_back(Clause(rel, { firstArg, secondArg }));
    return true;
}

bool QueryPreprocessor::checkSuchThatClause(string rel, vector<string> args) {
    if (this->validSuchThatArgType.find(rel) == this->validSuchThatArgType.end()) {
        this->isValid = false;
        return false;
    }

    vector<unordered_set<string>> validArgType = this->validSuchThatArgType.find(rel)->second;
    for (int i = 0; i < 2; i++) {
        if (validArgType[i].find(getArgType(args[i], this->declarations)) == validArgType[i].end()) {
            this->isValid = false;
            return false;
        }
    }

    return true;
}

bool QueryPreprocessor::parsePatternClause(string clause) {
    if (!regex_match(clause, regex("^.*\\(.*,.*\\)$"))) {
        this->isValid = false;
        return false;
    }

    int left = clause.find('(');
    int comma = clause.find(',');
    int right = clause.find(')');

    string syn = trim(clause.substr(0, left));
    string firstArg = trim(clause.substr(left + 1, comma - left - 1));
    string secondArg = trim(clause.substr(comma + 1, right - comma - 1));
    if (!checkPatternClause(syn, { firstArg, secondArg })) {
        this->isValid = false;
        return false;
    }

    this->clauses.push_back(Clause(syn, { firstArg, secondArg }));
    return true;
}

bool QueryPreprocessor::checkPatternClause(string syn, vector<string> args) {
    string argType = getArgType(syn, this->declarations);
    if (argType != ASSIGN_) {
        this->isValid = false;
        return false;
    }
    vector<unordered_set<string>> validArgType = this->validPatternArgType.find(argType)->second;
    for (int i = 0; i < 2; i++) {
        if (validArgType[i].find(getArgType(args[i], this->declarations)) == validArgType[i].end()) {
            this->isValid = false;
            return false;
        }
    }
    return true;
}

QueryPreprocessor::~QueryPreprocessor() {

}
