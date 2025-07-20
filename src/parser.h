#pragma once

#include "lexer.h"

#include "ast.h"
#include <map>
#include <memory>

class Parser {
    Lexer& lexer;
    Token currentToken;

public:
    Parser(Lexer& lexer);
    std::vector<std::unique_ptr<StmtAST>> parse();

private:
    void eat(TokenType type);
    std::unique_ptr<StmtAST> parseStatement();
    std::unique_ptr<StmtAST> parseAssignment();
    std::unique_ptr<StmtAST> parsePrint();
    std::unique_ptr<StmtAST> parseIf();
    std::unique_ptr<StmtAST> parseWhile();
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<ExprAST> parseTerm();
    std::unique_ptr<ExprAST> parseFactor();
    std::unique_ptr<ExprAST> parseCondition();
};
