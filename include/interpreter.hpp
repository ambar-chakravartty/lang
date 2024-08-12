#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Values.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>
#include "Environment.hpp"


class Interpreter{
    private:
        std::unique_ptr<RuntimeVal> evaluate(std::unique_ptr<Expr>& e,Environment& env);
        std::unique_ptr<RuntimeVal> evalNumericBinary(NumberVal* lhs,std::string op,NumberVal* rhs,Environment& e);
        std::unique_ptr<RuntimeVal> evalBinary(BinaryExpr* b,Environment& e);
	    std::unique_ptr<RuntimeVal> assignment(Assignment* node,Environment& e);
        std::unique_ptr<RuntimeVal> evaluate(Expr* node,Environment& e);
        void printStmt(std::unique_ptr<RuntimeVal> res,Environment& e);
        std::unique_ptr<RuntimeVal> evalStmt(Stmt* s,Environment& e);
        std::unique_ptr<RuntimeVal> varExpr(Identifier* node,Environment& e);
        std::unique_ptr<RuntimeVal> executeBlock(Block* b,Environment& e);
        bool isTruthy(RuntimeVal* val);



    public:
    
        std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Stmt>>& program,Environment& e);

        // Interpreter();



};


#endif
