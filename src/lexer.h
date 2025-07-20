#pragma once

#include "token.h"
#include <string>
#include <map>

class Lexer {
    std::string input;
    size_t position;
    int line;
    int column;
    std::map<std::string, TokenType> keywords;

public:
    Lexer(const std::string& input);
    Token nextToken();

private:
    char peek() const;
    void advance();
    void skipWhitespace();
};
