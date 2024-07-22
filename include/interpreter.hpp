#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Values.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>

std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Stmt>>& program);

#endif