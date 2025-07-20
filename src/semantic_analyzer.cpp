#include "semantic_analyzer.h"
#include <iostream>

bool SemanticAnalyzer::analyze(const std::vector<std::unique_ptr<StmtAST>>& statements) {
    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
    return !hasError;
}

void SemanticAnalyzer::visit(NumberExprAST& node) {}

void SemanticAnalyzer::visit(StringExprAST& node) {}

void SemanticAnalyzer::visit(VariableExprAST& node) {
    if (declaredVariables.find(node.getName()) == declaredVariables.end()) {
        std::cerr << "Semantic Error: Use of undeclared variable '" << node.getName() << "'" << std::endl;
        hasError = true;
    }
}

void SemanticAnalyzer::visit(BinaryExprAST& node) {
    node.getLHS().accept(*this);
    node.getRHS().accept(*this);
}

void SemanticAnalyzer::visit(AssignStmtAST& node) {
    node.getValue().accept(*this);
    declaredVariables.insert(node.getName());
}

void SemanticAnalyzer::visit(PrintStmtAST& node) {
    node.getValue().accept(*this);
}

void SemanticAnalyzer::visit(IfStmtAST& node) {
    node.getCondition().accept(*this);
    for (const auto& stmt : node.getThen()) {
        stmt->accept(*this);
    }
    for (const auto& stmt : node.getElse()) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(WhileStmtAST& node) {
    node.getCondition().accept(*this);
    for (const auto& stmt : node.getBody()) {
        stmt->accept(*this);
    }
}
