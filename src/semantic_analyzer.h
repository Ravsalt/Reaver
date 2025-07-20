#pragma once

#include "visitor.h"
#include "ast.h"
#include <vector>
#include <string>
#include <set>
#include <memory>

class SemanticAnalyzer : public Visitor {
    std::set<std::string> declaredVariables;
    bool hasError = false;

public:
    bool analyze(const std::vector<std::unique_ptr<StmtAST>>& statements);

    void visit(NumberExprAST& node) override;
    void visit(VariableExprAST& node) override;
    void visit(BinaryExprAST& node) override;
    void visit(StringExprAST& node) override;
    void visit(AssignStmtAST& node) override;
    void visit(PrintStmtAST& node) override;
    void visit(IfStmtAST& node) override;
    void visit(WhileStmtAST& node) override;
};
