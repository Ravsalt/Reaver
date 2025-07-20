#pragma once

#include <string>
#include <vector>
#include <memory>

#include "visitor.h"

// Base class for all nodes
class Node {
public:
    virtual ~Node() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// Base class for all expression nodes
class ExprAST : public Node {};

// Expression class for numeric literals like "1.0"
class NumberExprAST : public ExprAST {
    int value;
public:
    NumberExprAST(int value) : value(value) {}
    int getValue() const { return value; }
    void accept(Visitor& visitor) override;
};

// Expression class for referencing a variable, like "a"
class VariableExprAST : public ExprAST {
    std::string name;
public:
    VariableExprAST(const std::string& name) : name(name) {}
    const std::string& getName() const { return name; }
    void accept(Visitor& visitor) override;
};

// Expression class for a binary operator
class BinaryExprAST : public ExprAST {
    char op;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
        : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    char getOp() const { return op; }
    ExprAST& getLHS() const { return *LHS; }
    ExprAST& getRHS() const { return *RHS; }
    void accept(Visitor& visitor) override;
};

// Expression class for string literals
class StringExprAST : public ExprAST {
    std::string value;
public:
    StringExprAST(const std::string& value) : value(value) {}
    const std::string& getValue() const { return value; }
    void accept(Visitor& visitor) override;
};

// Base class for all statement nodes
class StmtAST : public Node {};

// Statement class for assignments
class AssignStmtAST : public StmtAST {
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    AssignStmtAST(const std::string& name, std::unique_ptr<ExprAST> value)
        : name(name), value(std::move(value)) {}
    const std::string& getName() const { return name; }
    ExprAST& getValue() const { return *value; }
    void accept(Visitor& visitor) override;
};

// Statement class for print
class PrintStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> value;
public:
    PrintStmtAST(std::unique_ptr<ExprAST> value) : value(std::move(value)) {}
    ExprAST& getValue() const { return *value; }
    void accept(Visitor& visitor) override;
};

// Statement class for if-else
class IfStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> condition;
    std::vector<std::unique_ptr<StmtAST>> thenBlock;
    std::vector<std::unique_ptr<StmtAST>> elseBlock;
public:
    IfStmtAST(std::unique_ptr<ExprAST> condition, 
              std::vector<std::unique_ptr<StmtAST>> thenBlock, 
              std::vector<std::unique_ptr<StmtAST>> elseBlock)
        : condition(std::move(condition)), thenBlock(std::move(thenBlock)), elseBlock(std::move(elseBlock)) {}

    ExprAST& getCondition() const { return *condition; }
    const std::vector<std::unique_ptr<StmtAST>>& getThen() const { return thenBlock; }
    const std::vector<std::unique_ptr<StmtAST>>& getElse() const { return elseBlock; }
    void accept(Visitor& visitor) override;
};

// Statement class for while loops
class WhileStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> condition;
    std::vector<std::unique_ptr<StmtAST>> body;
public:
    WhileStmtAST(std::unique_ptr<ExprAST> condition, std::vector<std::unique_ptr<StmtAST>> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    ExprAST& getCondition() const { return *condition; }
    const std::vector<std::unique_ptr<StmtAST>>& getBody() const { return body; }
    void accept(Visitor& visitor) override;
};
