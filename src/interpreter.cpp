#include "interpreter.h"
#include <iostream>

void Interpreter::interpret(const std::vector<std::unique_ptr<StmtAST>>& statements) {
    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
}

void Interpreter::visit(NumberExprAST& node) {
    lastValue = node.getValue();
}

void Interpreter::visit(VariableExprAST& node) {
    if (variables.find(node.getName()) != variables.end()) {
        lastValue = variables[node.getName()];
    } else {
        std::cerr << "Undefined variable: " << node.getName() << std::endl;
        exit(1);
    }
}

void Interpreter::visit(BinaryExprAST& node) {
    node.getLHS().accept(*this);
    int left = lastValue;
    node.getRHS().accept(*this);
    int right = lastValue;

    switch (node.getOp()) {
        case '<':
            lastValue = left < right;
            break;
        case '+':
            lastValue = left + right;
            break;
        case '-':
            lastValue = left - right;
            break;
        case '*':
            lastValue = left * right;
            break;
        case '/':
            if (right == 0) {
                std::cerr << "Runtime Error: Division by zero." << std::endl;
                exit(1);
            }
            lastValue = left / right;
            break;
        default:
            std::cerr << "Unsupported binary operator: " << node.getOp() << std::endl;
            exit(1);
    }
}

void Interpreter::visit(StringExprAST& node) {
    // For now, we can't store string values, so we'll just print them.
    std::cout << "Print: " << node.getValue() << std::endl;
}

void Interpreter::visit(AssignStmtAST& node) {
    node.getValue().accept(*this);
    variables[node.getName()] = lastValue;
    std::cout << "Assigned " << node.getName() << " = " << lastValue << std::endl;
}

void Interpreter::visit(PrintStmtAST& node) {
    node.getValue().accept(*this);
    // If it's a string, it will be handled by StringExprAST's visit method.
    // If it's a number or variable, we print the lastValue.
    if (dynamic_cast<StringExprAST*>(&node.getValue()) == nullptr) {
        std::cout << "Print: " << lastValue << std::endl;
    }
}

void Interpreter::visit(IfStmtAST& node) {
    node.getCondition().accept(*this);
    if (lastValue) { // if condition is true
        for (const auto& stmt : node.getThen()) {
            stmt->accept(*this);
        }
    } else { // if condition is false
        for (const auto& stmt : node.getElse()) {
            stmt->accept(*this);
        }
    }
}

void Interpreter::visit(WhileStmtAST& node) {
    node.getCondition().accept(*this);
    while (lastValue) {
        for (const auto& stmt : node.getBody()) {
            stmt->accept(*this);
        }
        node.getCondition().accept(*this);
    }
}
