#pragma once

#include "visitor.h"
#include "ast.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class Interpreter : public Visitor {
    std::map<std::string, int> variables;
    int lastValue;

public:
    void interpret(const std::vector<std::unique_ptr<StmtAST>>& statements);

    void visit(NumberExprAST& node) override;
    void visit(VariableExprAST& node) override;
    void visit(BinaryExprAST& node) override;
    void visit(StringExprAST& node) override;
    void visit(AssignStmtAST& node) override;
    void visit(PrintStmtAST& node) override;
    void visit(IfStmtAST& node) override;
    void visit(WhileStmtAST& node) override;
};
