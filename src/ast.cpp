#include "ast.h"

void NumberExprAST::accept(Visitor& visitor) { visitor.visit(*this); }
void VariableExprAST::accept(Visitor& visitor) { visitor.visit(*this); }
void BinaryExprAST::accept(Visitor& visitor) { visitor.visit(*this); }
void StringExprAST::accept(Visitor& visitor) { visitor.visit(*this); }
void AssignStmtAST::accept(Visitor& visitor) { visitor.visit(*this); }
void PrintStmtAST::accept(Visitor& visitor) { visitor.visit(*this); }
void IfStmtAST::accept(Visitor& visitor) { visitor.visit(*this); }
void WhileStmtAST::accept(Visitor& visitor) { visitor.visit(*this); }
