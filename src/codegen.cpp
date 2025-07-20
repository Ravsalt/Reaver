
#include "codegen.h"
#include <functional>

CodeGenerator::CodeGenerator(const std::string& filename) : outputFile(filename) {}

std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(labelCounter++);
}

int CodeGenerator::getVariableOffset(const std::string& name) {
    if (variableOffsets.find(name) == variableOffsets.end()) {
        stackIndex -= 8; // 8 bytes for a 64-bit integer
        variableOffsets[name] = stackIndex;
    }
    return variableOffsets[name];
}

void CodeGenerator::generate(const std::vector<std::unique_ptr<StmtAST>>& statements) {
    // Boilerplate assembly header
    outputFile << "extern print_integer" << std::endl;
    outputFile << "extern print_string" << std::endl;
    outputFile << "section .data" << std::endl;
    
    // Recursively find all string literals and declare them in the .data section
    int stringCounter = 0;
    std::function<void(const StmtAST*)> findStrings = 
        [&](const StmtAST* stmt) {
        if (auto printStmt = dynamic_cast<const PrintStmtAST*>(stmt)) {
            if (auto strExpr = dynamic_cast<const StringExprAST*>(&printStmt->getValue())) {
                outputFile << "    str" << stringCounter++ << ": db \"" << strExpr->getValue() << "\", 0" << std::endl;
            }
        }
        // Recurse into control flow blocks
        if (auto ifStmt = dynamic_cast<const IfStmtAST*>(stmt)) {
            for (const auto& s : ifStmt->getThen()) findStrings(s.get());
            for (const auto& s : ifStmt->getElse()) findStrings(s.get());
        }
        if (auto whileStmt = dynamic_cast<const WhileStmtAST*>(stmt)) {
            for (const auto& s : whileStmt->getBody()) findStrings(s.get());
        }
    };

    for (const auto& stmt : statements) {
        findStrings(stmt.get());
    }

    outputFile << "section .text" << std::endl;
    outputFile << "global main" << std::endl;
    outputFile << "main:" << std::endl;
    outputFile << "    push rbp" << std::endl;
    outputFile << "    mov rbp, rsp" << std::endl;

    // Allocate space for variables on the stack
    outputFile << "    sub rsp, 256" << std::endl; 

    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }

    // Boilerplate assembly footer
    outputFile << "    mov rsp, rbp" << std::endl;
    outputFile << "    pop rbp" << std::endl;
    outputFile << "    mov rax, 60" << std::endl; // syscall for exit
    outputFile << "    xor rdi, rdi" << std::endl; // exit code 0
    outputFile << "    syscall" << std::endl;
}

void CodeGenerator::visit(NumberExprAST& node) {
    outputFile << "    push " << node.getValue() << std::endl;
}

void CodeGenerator::visit(VariableExprAST& node) {
    int offset = getVariableOffset(node.getName());
    outputFile << "    push qword [rbp" << offset << "]" << std::endl;
}

void CodeGenerator::visit(BinaryExprAST& node) {
    node.getLHS().accept(*this);
    node.getRHS().accept(*this);

    outputFile << "    pop rbx" << std::endl; // RHS
    outputFile << "    pop rax" << std::endl; // LHS

    switch (node.getOp()) {
        case '+':
            outputFile << "    add rax, rbx" << std::endl;
            break;
        case '-':
            outputFile << "    sub rax, rbx" << std::endl;
            break;
        case '*':
            outputFile << "    imul rax, rbx" << std::endl;
            break;
        case '/':
            outputFile << "    cqo" << std::endl; // sign-extend rax to rdx:rax
            outputFile << "    idiv rbx" << std::endl;
            break;
        case '<':
            outputFile << "    cmp rax, rbx" << std::endl;
            outputFile << "    setl al" << std::endl; // set al to 1 if less, 0 otherwise
            outputFile << "    movzx rax, al" << std::endl; // zero-extend al to rax
            break;
    }
    outputFile << "    push rax" << std::endl;
}

void CodeGenerator::visit(StringExprAST& node) {
    static int stringCounter = 0;
    outputFile << "    mov rdi, str" << stringCounter++ << std::endl;
}

void CodeGenerator::visit(AssignStmtAST& node) {
    node.getValue().accept(*this);
    int offset = getVariableOffset(node.getName());
    outputFile << "    pop qword [rbp" << offset << "]" << std::endl;
}

void CodeGenerator::visit(PrintStmtAST& node) {
    if (dynamic_cast<StringExprAST*>(&node.getValue())) {
        node.getValue().accept(*this);
        // Align stack for stdio call (must be 16-byte aligned before call)
        outputFile << "    sub rsp, 8" << std::endl;
        outputFile << "    mov rax, 0" << std::endl; // No vector registers used for variadic printf
        outputFile << "    call print_string" << std::endl;
        outputFile << "    add rsp, 8" << std::endl; // Restore stack pointer
    } else {
        node.getValue().accept(*this);
        outputFile << "    pop rdi" << std::endl;
        // Align stack for stdio call (must be 16-byte aligned before call)
        outputFile << "    sub rsp, 8" << std::endl;
        outputFile << "    mov rax, 0" << std::endl; // No vector registers used for variadic printf
        outputFile << "    call print_integer" << std::endl;
        outputFile << "    add rsp, 8" << std::endl; // Restore stack pointer
    }
}

void CodeGenerator::visit(IfStmtAST& node) {
    std::string elseLabel = newLabel();
    std::string endLabel = newLabel();

    node.getCondition().accept(*this);
    outputFile << "    pop rax" << std::endl;
    outputFile << "    test rax, rax" << std::endl;
    outputFile << "    jz " << elseLabel << std::endl;

    // Then block
    for (const auto& stmt : node.getThen()) {
        stmt->accept(*this);
    }
    outputFile << "    jmp " << endLabel << std::endl;

    // Else block
    outputFile << elseLabel << ":" << std::endl;
    for (const auto& stmt : node.getElse()) {
        stmt->accept(*this);
    }

    outputFile << endLabel << ":" << std::endl;
}

void CodeGenerator::visit(WhileStmtAST& node) {
    std::string startLabel = newLabel();
    std::string endLabel = newLabel();

    outputFile << startLabel << ":" << std::endl;
    node.getCondition().accept(*this);
    outputFile << "    pop rax" << std::endl;
    outputFile << "    test rax, rax" << std::endl;
    outputFile << "    jz " << endLabel << std::endl;

    for (const auto& stmt : node.getBody()) {
        stmt->accept(*this);
    }
    outputFile << "    jmp " << startLabel << std::endl;

    outputFile << endLabel << ":" << std::endl;
}
