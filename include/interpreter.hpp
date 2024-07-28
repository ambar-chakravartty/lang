#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Values.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>
#include "Environment.hpp"


class Interpreter{
    private:
        Environment env;
        std::unique_ptr<RuntimeVal> evaluate(std::unique_ptr<Expr>& e);
        std::unique_ptr<RuntimeVal> evalNumericBinary(NumberVal* lhs,std::string op,NumberVal* rhs);
        std::unique_ptr<RuntimeVal> evalBinary(BinaryExpr* b);
        std::unique_ptr<RuntimeVal> evaluate(Expr* node);
        void printStmt(std::unique_ptr<RuntimeVal> res);
        std::unique_ptr<RuntimeVal> evalStmt(Stmt* s);
        std::unique_ptr<RuntimeVal> varExpr(Identifier* node);



    public:
    
        std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Stmt>>& program);

        // Interpreter();



};


#endif