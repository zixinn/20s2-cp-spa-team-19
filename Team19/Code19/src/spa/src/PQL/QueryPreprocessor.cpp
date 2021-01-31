#include <regex>

#include "QueryPreprocessor.h"
#include "Utility.h"

QueryPreprocessor::QueryPreprocessor() {
    designEntities = { PROCEDURE, STMTLST, STMT, READ, PRINT, ASSIGN, CALL, WHILE, IF, VARIABLE, CONSTANT };
    validSuchThatArgType["Follows"] = { { STMT, READ, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE },
                                   { STMT, READ, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE } };
    validSuchThatArgType["Follows*"] = { { STMT, READ, PRINT, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE },
                                   { STMT, READ, PRINT, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE } };
    validSuchThatArgType["Parent"] = { { STMT, WHILE, IF, INTEGER, UNDERSCORE },
                                   { STMT, READ, PRINT, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE } };
    validSuchThatArgType["Parent*"] = { { STMT, WHILE, IF, INTEGER, UNDERSCORE },
                                   { STMT, READ, PRINT, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, UNDERSCORE } };
    validSuchThatArgType["Uses"] = { { STMT, PRINT, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, NAME },
                                   { VARIABLE, NAME, UNDERSCORE } };
    validSuchThatArgType["Modifies"] = { { STMT, READ, PROCEDURE, ASSIGN, CALL, WHILE, IF, INTEGER, NAME },
                                { VARIABLE, NAME, UNDERSCORE } };
    validPatternArgType["assign"] = { { VARIABLE, NAME, UNDERSCORE },
                                { UNDERSCORE, NAME, EXPRESSION, EXPRESSIONWITHUNDERSCORE } };
}

bool QueryPreprocessor::checkSynonymDeclared(string synonym) {
    return this->declarations.find(synonym) != this->declarations.end();
}

string QueryPreprocessor::getArgType(string synonym) {
    if (checkSynonymDeclared(synonym)) {
        return this->declarations[synonym];
    } else if (checkInteger(synonym)) {
        return INTEGER;
    } else if (synonym == "_") {
        return UNDERSCORE;
    } else if (checkNameWithQuotes(synonym)) {
        return NAME;
    } else if (checkExpression(synonym)) {
        return EXPRESSION;
    } else if (checkExpressionWithUnderscores(synonym)) {
        return EXPRESSIONWITHUNDERSCORE;
    } else {
        return "";
    }
}

Query QueryPreprocessor::process(string query) {
    this->declarations.clear();
    this->toSelect = "";
    this->suchThatClauses.clear();
    this->patternClauses.clear();
    this->isValid = true;

    vector<string> statements = split(query, ";");

    bool selectFound = false;
    bool multipleSelect = false;
    for (int i = 0; i < statements.size(); i++) {
        if (statements[i].find("Select ") == 0) {
            if (selectFound) {
                multipleSelect = true;
                break;
            }
            selectFound = true;
        }
    }
    if (!selectFound || multipleSelect) {
        this->isValid = false;
    }

    for (int i = 0; i < statements.size() && this->isValid; i++) {
        if (statements[i].find("Select ") != 0) {
            if (!regex_match(statements[i], regex("^.*\\s.*$"))) {
                this->isValid = false;
                break;
            }

            int space = statements[i].find(' ');
            string designEntity = trim(statements[i].substr(0, space));
            string synonyms = trim(statements[i].substr(space + 1));
            parseDeclaration(designEntity, synonyms);
        } else {
            parseSelect(trim(statements[i].substr(7)));
        }
    }

    return Query(this->declarations, this->toSelect, this->suchThatClauses, this->patternClauses, this->isValid);
}

bool QueryPreprocessor::parseDeclaration(string designEntity, string synonyms) {
    if (!checkDesignEntity(designEntity)) {
        this->isValid = false;
        return false;
    }
    vector<string> synonymsVector = split(synonyms, ",");
    for (string synonym : synonymsVector) {
        if (checkSynonymDeclared(synonym) || !checkName(synonym)) {
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

    if (suchThatPos == string::npos && patternPos == string::npos) { // no such that and pattern clause
        return parseToSelect(select);
    } else if (patternPos == string::npos) { // no pattern clause
        return parseToSelect(trim(select.substr(0, suchThatPos)))
                && parseSuchThatClause(trim(select.substr(suchThatPos + 11)));
    } else if (suchThatPos == string::npos) { // no such that clause
        return parseToSelect(trim(select.substr(0, patternPos)))
               && parsePatternClause(trim(select.substr(patternPos + 9)));
    } else { // both such that and pattern clause
        int minPos = min(suchThatPos, patternPos);
        if (suchThatPos < patternPos) { // such that before pattern
            return parseToSelect(trim(select.substr(0, suchThatPos)))
                   && parseSuchThatClause(trim(select.substr(suchThatPos + 11, patternPos - suchThatPos - 11)))
                   && parsePatternClause(trim(select.substr(patternPos + 9)));
        } else { // pattern before such that
            return parseToSelect(trim(select.substr(0, patternPos)))
                   && parsePatternClause(trim(select.substr(patternPos + 9, suchThatPos - patternPos - 9)))
                   && parseSuchThatClause(trim(select.substr(suchThatPos + 11)));
        }
    }
}

bool QueryPreprocessor::parseToSelect(string synonym) {
    if (!checkSynonymDeclared(synonym) || !checkName(synonym)) {
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

    this->suchThatClauses.push_back(Clause(rel, { firstArg, secondArg }));
    return true;
}

bool QueryPreprocessor::checkSuchThatClause(string rel, vector<string> args) {
    if (this->validSuchThatArgType.find(rel) == this->validSuchThatArgType.end()) {
        this->isValid = false;
        return false;
    }

    vector<unordered_set<string>> validArgType = this->validSuchThatArgType.find(rel)->second;
    for (int i = 0; i < 2; i++) {
        if (validArgType[i].find(getArgType(args[i])) == validArgType[i].end()) {
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

    this->patternClauses.push_back(Clause(syn, { firstArg, secondArg }));
    return true;
}

bool QueryPreprocessor::checkPatternClause(string syn, vector<string> args) {
    string argType = getArgType(syn);
    if (argType != "assign") {
        this->isValid = false;
        return false;
    }
    vector<unordered_set<string>> validArgType = this->validPatternArgType.find(argType)->second;
    for (int i = 0; i < 2; i++) {
        if (validArgType[i].find(getArgType(args[i])) == validArgType[i].end()) {
            this->isValid = false;
            return false;
        }
    }
    return true;
}

QueryPreprocessor::~QueryPreprocessor() {

}
