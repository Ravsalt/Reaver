#include "parser.h"
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

void Parser::eat(TokenType type) {
    if (currentToken.type == type) {
        currentToken = lexer.nextToken();
    } else {
        std::cerr << "Unexpected token: '" << currentToken.value 
                  << "' at line " << currentToken.line 
                  << ", column " << currentToken.column << std::endl;
        exit(1);
    }
}

std::vector<std::unique_ptr<StmtAST>> Parser::parse() {
    std::vector<std::unique_ptr<StmtAST>> statements;
    while (currentToken.type != TokenType::END) {
        statements.push_back(parseStatement());
    }
    return statements;
}

std::unique_ptr<StmtAST> Parser::parseStatement() {
    switch (currentToken.type) {
        case TokenType::IDENTIFIER:
            return parseAssignment();
        case TokenType::RPRINT:
            return parsePrint();
        case TokenType::IF:
            return parseIf();
        case TokenType::WHILE:
            return parseWhile();
        default:
            std::cerr << "Unexpected statement starting with token '" << currentToken.value << "'" << std::endl;
            exit(1);
    }
}

std::unique_ptr<StmtAST> Parser::parseAssignment() {
    std::string varName = currentToken.value;
    eat(TokenType::IDENTIFIER);
    eat(TokenType::EQUALS);
    auto value = parseExpression();
    return std::unique_ptr<AssignStmtAST>(new AssignStmtAST(varName, std::move(value)));
}

std::unique_ptr<StmtAST> Parser::parsePrint() {
    eat(TokenType::RPRINT);
    auto value = parseExpression();
    return std::unique_ptr<PrintStmtAST>(new PrintStmtAST(std::move(value)));
}

std::unique_ptr<ExprAST> Parser::parseFactor() {
    Token token = currentToken;
    if (token.type == TokenType::INTEGER) {
        eat(TokenType::INTEGER);
        return std::unique_ptr<NumberExprAST>(new NumberExprAST(std::stoi(token.value)));
    } else if (token.type == TokenType::IDENTIFIER) {
        eat(TokenType::IDENTIFIER);
        return std::unique_ptr<VariableExprAST>(new VariableExprAST(token.value));
    } else if (token.type == TokenType::STRING) {
        eat(TokenType::STRING);
        return std::unique_ptr<StringExprAST>(new StringExprAST(token.value));
    } else if (token.type == TokenType::LPAREN) {
        eat(TokenType::LPAREN);
        auto expr = parseExpression();
        eat(TokenType::RPAREN);
        return expr;
    }

    std::cerr << "Invalid factor starting with token '" << token.value << "'" << std::endl;
    exit(1);
}

std::unique_ptr<ExprAST> Parser::parseTerm() {
    auto node = parseFactor();

    while (currentToken.type == TokenType::STAR || currentToken.type == TokenType::SLASH) {
        char op = (currentToken.type == TokenType::STAR) ? '*' : '/';
        eat(currentToken.type);
        auto rhs = parseFactor();
        node = std::unique_ptr<BinaryExprAST>(new BinaryExprAST(op, std::move(node), std::move(rhs)));
    }

    return node;
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
    auto node = parseTerm();

    while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
        char op = (currentToken.type == TokenType::PLUS) ? '+' : '-';
        eat(currentToken.type);
        auto rhs = parseTerm();
        node = std::unique_ptr<BinaryExprAST>(new BinaryExprAST(op, std::move(node), std::move(rhs)));
    }

    return node;
}

std::unique_ptr<ExprAST> Parser::parseCondition() {
    auto left = parseExpression();
    
    char op;
    if (currentToken.type == TokenType::LESS_THAN) {
        op = '<';
    } else {
        std::cerr << "Unsupported operator in condition: " << currentToken.value << std::endl;
        exit(1);
    }
    eat(currentToken.type);
    
    auto right = parseExpression();
    
    return std::unique_ptr<BinaryExprAST>(new BinaryExprAST(op, std::move(left), std::move(right)));
}

std::unique_ptr<StmtAST> Parser::parseIf() {
    eat(TokenType::IF);
    auto condition = parseCondition();
    eat(TokenType::LBRACE);

    std::vector<std::unique_ptr<StmtAST>> thenBlock;
    while (currentToken.type != TokenType::RBRACE && currentToken.type != TokenType::END) {
        thenBlock.push_back(parseStatement());
    }
    eat(TokenType::RBRACE);

    std::vector<std::unique_ptr<StmtAST>> elseBlock;
    if (currentToken.type == TokenType::ELSE) {
        eat(TokenType::ELSE);
        eat(TokenType::LBRACE);
        while (currentToken.type != TokenType::RBRACE && currentToken.type != TokenType::END) {
            elseBlock.push_back(parseStatement());
        }
        eat(TokenType::RBRACE);
    }

    return std::unique_ptr<IfStmtAST>(new IfStmtAST(std::move(condition), std::move(thenBlock), std::move(elseBlock)));
}

std::unique_ptr<StmtAST> Parser::parseWhile() {
    eat(TokenType::WHILE);
    auto condition = parseCondition();
    eat(TokenType::LBRACE);

    std::vector<std::unique_ptr<StmtAST>> body;
    while (currentToken.type != TokenType::RBRACE && currentToken.type != TokenType::END) {
        body.push_back(parseStatement());
    }
    eat(TokenType::RBRACE);

    return std::unique_ptr<WhileStmtAST>(new WhileStmtAST(std::move(condition), std::move(body)));
}
