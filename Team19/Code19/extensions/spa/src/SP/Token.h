#pragma once
#include "../AbstractAPI.h"

using namespace std;

// Token representations of SIMPLE source syntax
namespace sp {
    class Token {
    public:
        enum class TokenType {
            ERROR, EOFF,
            PROC, READ, PRINT, CALL, WHILE, IF, THEN, ELSE, ASSIGN,  //keywords
            NAME, CONST,   //identifiers
            PLUS, MINUS, TIMES, DIV, MOD,  //operators
            AND, OR,  //infix cond
            EQ, NEQ, GT, GTE, LT, LTE,  //rel expr
            NOT,  //prefix cond
            SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE,  //delimiters
            PROCNAME, CONSTNAME,			// for parser use only
            BOOL,							// for parser use only
        };

        //default constructor for initialisation
        Token();
        ~Token();

        Token(TokenType type, STRING literal);

        TokenType getType();
        STRING getLiteral();
        bool compare(Token* other);

    private:
        TokenType type;
        STRING literal;
    };
    struct tokentype_hash { //hash<Token::TokenType> {
        size_t operator() (Token::TokenType const& x) const { return std::hash<int>()(int(x));  }
    };
}
