#pragma once
#include "../AbstractAPI.h"
#include <unordered_set>
#include <vector>
#include <iostream>
#include "SP/Token.h"
#include "AST/CondExpr.h"
#include "AST/VarName.h"
#include "AST/ConstVal.h"
#include "SP/ParserUtils.h"

// Interface CondExpr for booleans
namespace ast {
    class CondExprBag : public CondExpr
    {
        std::unordered_set<STRING> var_names;
        std::unordered_set<STRING> const_vals;
        std::vector<sp::Token*> tokens;
    public:
        CondExprBag(sp::Token* token) :CondExpr(token) {};
        inline STRING toString() {
            STRING out = "";
            for (int i = 0; i < tokens.size(); ++i) {
                out += tokens[i]->getLiteral();
                if (i < tokens.size() - 1) {
                    out += " ";
                }
            }
            return out;
        };
        inline std::vector<STRING> getVarNames() {
            std::vector<STRING> vns;
            for (STRING vn : this->var_names) {
                vns.push_back(vn);
            }
            return vns;
        }
        inline std::vector<STRING> getConstVal() {
            std::vector<STRING> cvs;
            for (STRING cv : this->const_vals) {
                cvs.push_back(cv);
            }
            return cvs;
        }
        inline std::vector<sp::Token*> getTokens() { return this->tokens; }
        inline void pushVarName(STRING vn) { this->var_names.insert(vn);  }
        inline void pushConstVal(STRING cv) { this->const_vals.insert(cv);  }
        inline void pushToken(sp::Token* tok) {
            if (tok->getType() == sp::Token::TokenType::NAME || ParserUtils::isKeyword(tok->getType())) {
                auto vn = tok->getLiteral();
                pushVarName(vn);
            }
            else if (tok->getType() == sp::Token::TokenType::CONST) {
                auto cv = tok->getLiteral();
                pushConstVal(cv);
            }
            this->tokens.push_back(tok);
        }
        ~CondExprBag() {
            var_names.clear();
            const_vals.clear();
            for (int i = 0; i < tokens.size(); ++i) {
                delete tokens[i];
            }
            const_vals.clear();
        }
    };
}
