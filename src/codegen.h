
#pragma once

#include "visitor.h"
#include "ast.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

class CodeGenerator : public Visitor {
    std::ofstream outputFile;
    int labelCounter = 0;
    std::map<std::string, int> variableOffsets;
    int stackIndex = 0;
    int stringVisitCounter = 0;

public:
    CodeGenerator(const std::string& filename);
    void generate(const std::vector<std::unique_ptr<StmtAST>>& statements);

    void visit(NumberExprAST& node) override;
    void visit(VariableExprAST& node) override;
    void visit(BinaryExprAST& node) override;
    void visit(StringExprAST& node) override;
    void visit(AssignStmtAST& node) override;
    void visit(PrintStmtAST& node) override;
    void visit(IfStmtAST& node) override;
    void visit(WhileStmtAST& node) override;

private:
    std::string newLabel();
    int getVariableOffset(const std::string& name);
};
