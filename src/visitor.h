#pragma once

// Forward declarations of AST nodes
class NumberExprAST;
class VariableExprAST;
class BinaryExprAST;
class StringExprAST;
class AssignStmtAST;
class PrintStmtAST;
class IfStmtAST;
class WhileStmtAST;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(NumberExprAST& node) = 0;
    virtual void visit(VariableExprAST& node) = 0;
    virtual void visit(BinaryExprAST& node) = 0;
    virtual void visit(StringExprAST& node) = 0;
    virtual void visit(AssignStmtAST& node) = 0;
    virtual void visit(PrintStmtAST& node) = 0;
    virtual void visit(IfStmtAST& node) = 0;
    virtual void visit(WhileStmtAST& node) = 0;
};
