#pragma once

#include <string>

enum class TokenType {
    // Keywords
    IF,        // if
    ELSE,      // else
    WHILE,     // while
    PRINT,
    RPRINT,    // rp keyword
    
    // Literals
    INTEGER,
    STRING,
    IDENTIFIER,
    
    // Operators
    LESS_THAN, // <
    EQUALS,    // =
    PLUS,      // +
    MINUS,     // -
    STAR,      // *
    SLASH,     // /
    
    // Structure
    LBRACE,    // {
    RBRACE,    // }
    LPAREN,    // (
    RPAREN,    // )
    
    // End of file
    END,       // End of file
    
    // Error
    ERROR      // Error token
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};
