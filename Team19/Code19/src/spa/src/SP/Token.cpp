#include "Token.h"

namespace sp {
    Token::Token() {

    }

    Token::~Token() {

    }

    Token::Token(TokenType type, STRING literal) {
        this->type = type;
        this->literal = literal;
    }

    Token::TokenType Token::getType() {
        return this->type;
    }

    string Token::getLiteral() {
        return this->literal;
    }

    bool Token::compare(Token* other) {
        if (!other) {
            throw "Token comparison fails";
        }
        return this->literal == other->literal && this->type == other->type;
    }
}
