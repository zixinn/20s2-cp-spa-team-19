#pragma once
#include "../AbstractAPI.h"

#include <iostream>
#include <exception>

// A collection of custom exception types used by the Parser
namespace sp {
    class ParserException : public std::exception {
        const STRING type;
        const STRING info;
        const StmtNum stmtNum;

    public:
        ParserException(StmtNum stmtNum, STRING info) :type{ "" }, stmtNum{ stmtNum }, info{ info } {};
        ParserException(STRING type, StmtNum stmtNum, STRING info) :type{type} ,stmtNum { stmtNum }, info{ info } {};
        inline STRING what() {
            const STRING ex_header = this->type == "" ? "" : this->type + " :: ";
            if (stmtNum >= 0) {
                return ex_header + "ERROR :: StmtNum: " + std::to_string(this->stmtNum) + "\n - " + this->info + "\n";
            } else {
                //Program and Proc have no stmtnum
                return ex_header + "ERROR:: \n - " + this->info + "\n";
            }
        }

    };

    // overloading stream operator
    inline std::ostream& operator<<(std::ostream &strm, ParserException &a) {
        return strm << "A(" << a.what() << ")";
    }

    class ParseExprException : public ParserException {
    public:
        ParseExprException(StmtNum stmtNum, STRING info) :ParserException("Expr", stmtNum, info ) {};
        ParseExprException(STRING type, StmtNum stmtNum, STRING info) :ParserException(type, stmtNum, info ) {};
    };

    class ParseCondExprException : public ParseExprException {
    public:
        ParseCondExprException(StmtNum stmtNum, STRING info) :ParseExprException("CondExpr", stmtNum, info ) {};
    };

    class UtilsException : public std::exception {
        const STRING type;
        const STRING info;

    public:
        UtilsException(STRING info) :type{ "" }, info{ info } {};
        UtilsException(STRING type, STRING info) :type{type}, info{ info } {};
        inline STRING what() {
            const STRING ex_header = this->type == "" ? "" : this->type + " :: ";
            return ex_header + "ERROR :: \n - " + this->info + "\n";
        }

    };
}

