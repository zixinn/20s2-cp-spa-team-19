#pragma once

#include <iostream>
#include <string>
#include <exception>

namespace sp {
	class ParserException : public std::exception {
		const std::string type;
		const std::string info;
		const int stmtNum;

	public:
		ParserException(int stmtNum, std::string info) :type{ "" }, stmtNum{ stmtNum }, info{ info } {};
		ParserException(std::string type, int stmtNum, std::string info) :type{type} ,stmtNum { stmtNum }, info{ info } {};
		inline std::string what() {
			const std::string ex_header = this->type == "" ? "" : this->type + " :: ";
			return ex_header + "ERROR :: StmtNum: " + std::to_string(this->stmtNum) + "\n - " + this->info + "\n";
		}

	};

	// overloading stream operator
	inline std::ostream& operator<<(std::ostream &strm, ParserException &a) {
		return strm << "A(" << a.what() << ")";
	}

	class ParseExprException : public ParserException {
	public:
		ParseExprException(int stmtNum, std::string info) :ParserException("Expr", stmtNum, info ) {};
		ParseExprException(std::string type, int stmtNum, std::string info) :ParserException(type, stmtNum, info ) {};
	};

	class ParseCondExprException : public ParseExprException {
	public:
		ParseCondExprException(int stmtNum, std::string info) :ParseExprException("CondExpr", stmtNum, info ) {};
	};

	class UtilsException : public std::exception {
		const std::string type;
		const std::string info;

	public:
		UtilsException(std::string info) :type{ "" }, info{ info } {};
		UtilsException(std::string type, std::string info) :type{type}, info{ info } {};
		inline std::string what() {
			const std::string ex_header = this->type == "" ? "" : this->type + " :: ";
			return ex_header + "ERROR :: \n - " + this->info;
		}

	};
}

