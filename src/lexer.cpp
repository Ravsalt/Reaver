#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& input) : input(input), position(0), line(1), column(1) {
    // Initialize keywords
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["while"] = TokenType::WHILE;
    keywords["rp"] = TokenType::RPRINT;
}

char Lexer::peek() const {
    if (position >= input.length()) return '\0';
    return input[position];
}

void Lexer::advance() {
    if (peek() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) {
        advance();
    }
}

Token Lexer::nextToken() {
    while (true) {
        skipWhitespace();

        if (position >= input.length()) {
            return {TokenType::END, "", line, column};
        }

        // Handle comments
        if (peek() == '#') {
            while (peek() != '\n' && peek() != '\0') {
                advance();
            }
            continue; // Continue to the next token or whitespace
        }

        break; // Exit loop if not whitespace or comment
    }

    if (position >= input.length()) {
        return {TokenType::END, "", line, column};
    }

    char current = peek();

    // Handle numbers
    if (std::isdigit(current)) {
        std::string num;
        while (std::isdigit(peek())) {
            num += peek();
            advance();
        }
        return {TokenType::INTEGER, num, line, column};
    }

    // Handle identifiers and keywords
    if (std::isalpha(current) || current == '_') {
        std::string ident;
        while (std::isalnum(peek()) || peek() == '_') {
            ident += peek();
            advance();
        }

        auto it = keywords.find(ident);
        if (it != keywords.end()) {
            return {it->second, ident, line, column};
        }
        return {TokenType::IDENTIFIER, ident, line, column};
    }

    // Handle strings (for print statements)
    if (current == '"') {
        advance(); // Skip opening quote
        std::string str;
        while (peek() != '"' && peek() != '\0') {
            str += peek();
            advance();
        }
        if (peek() == '"') {
            advance(); // Skip closing quote
            return {TokenType::STRING, str, line, column};
        } else {
            return {TokenType::ERROR, "Unterminated string", line, column};
        }
    }

    // Handle operators and punctuation
    switch (current) {
        case '<':
            advance();
            return {TokenType::LESS_THAN, "<", line, column};
        case '=':
            advance();
            return {TokenType::EQUALS, "=", line, column};
        case '+':
            advance();
            return {TokenType::PLUS, "+", line, column};
        case '-':
            advance();
            return {TokenType::MINUS, "-", line, column};
        case '*':
            advance();
            return {TokenType::STAR, "*", line, column};
        case '/':
            advance();
            return {TokenType::SLASH, "/", line, column};
        case '{':
            advance();
            return {TokenType::LBRACE, "{", line, column};
        case '}':
            advance();
            return {TokenType::RBRACE, "}", line, column};
        case '(':
            advance();
            return {TokenType::LPAREN, "(", line, column};
        case ')':
            advance();
            return {TokenType::RPAREN, ")", line, column};
        default:
            advance();
            return {TokenType::ERROR, std::string("Unexpected character: ") + current, line, column};
    }
}
