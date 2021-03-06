#pragma once
#include "SP/Token.h"
#include "SP/Lexer.h"
#include "SP/Parser.h"
#include "SP/ParserException.h"

// function pointers and utility methods for parsing Expressions
namespace CondExprUtils {

    // helper functions
    STRING VectorToString(std::vector<sp::Token*>& input);
    void VectorSlice(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int right);
    void VectorShallowCopy(std::vector<sp::Token*> input, std::vector<sp::Token*>& output);

    // l_paren++ for LParen, l_paren-- for RParen
    // example start: x-1, start_lparen:  0, target_lparen:  1 := `( ...   left_start` for <
    // example start: x-1, start_lparen: -1, target_lparen:  0 := `( ... ) left_start` for &&
    int SeekParenLeft(std::vector<sp::Token*>& input, int left_start, int start_lparen, int target_lparen);
    int SeekParenRight(std::vector<sp::Token*>& input, int right_start, int start_lparen, int target_lparen);
    int SeekParenLeftCheck(std::vector<sp::Token*>& input, int left_start);
    int SeekParenRightCheck(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int right_start);
    int CopyAndReplace(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int index, int right);
    int CopyAndReplaceExcl(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int left, int index, int right);
    bool CheckSubExpr(std::vector<sp::Token*>& input);
    bool CheckSubExprNoThrow(std::vector<sp::Token*>& input);

    // for use in ParseRelExpr
    void CheckExpr(std::vector<sp::Token*> input, STRING comp);

    // expect leftmost and rightmost to be brackets
    void ParseRelExpr(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int index);

    // outermost logic
    void ParseCondExpr(std::vector<sp::Token*>& input);
    void ParseCondExprInner(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output);

    // called by ParseCondExpr, ParseNot, ParseAndOr
    void CondExprDispatch(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output);
    void RelExprDispatch(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output);

    void ParseNot(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int index);
    void ParseAndOr(std::vector<sp::Token*>& input, std::vector<sp::Token*>& output, int index);
}
