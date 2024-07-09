#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Values.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>

std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Expr>>& program);

// std::unique_ptr<RuntimeVal> evaluate(Expr* node);
std::unique_ptr<RuntimeVal> evaluate(std::unique_ptr<Expr>& e); //what's the difference between passing std::unique_ptr<Expr> and std::unique_ptr<Expr>& ???

std::unique_ptr<RuntimeVal> evalNumericBinary(NumberVal* lhs,std::string op,NumberVal* rhs);
std::unique_ptr<RuntimeVal> evalBinary(BinaryExpr* b);

std::unique_ptr<RuntimeVal> evaluate(Expr* node);

#endif