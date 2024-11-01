#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP


#include "ast.hpp"
#include <any>
#include <memory>
#include <stdexcept>
#include <vector>
#include "Environment.hpp"


class Interpreter{
    private:
        std::any evaluate(std::unique_ptr<Expr>& e,Environment* env);
        std::any evalBinary(BinaryExpr* b,Environment* e);
	    std::any assignment(Assignment* node,Environment* e);
        std::any evaluate(Expr* node,Environment* e);
        void printStmt(std::any res,Environment* e);
        std::any evalStmt(Stmt* s,Environment* e);
        std::any varExpr(Identifier* node,Environment* e);
        std::any executeBlock(Block* b,Environment* e);
        std::any funDeclaration(FunDecl* d,Environment* e);
	std::any evalIf(IfStmt* s,Environment* e);
        std::any call(CallExpr* c,Environment* e);
        std::any returnCall(ReturnStmt* s,Environment* e);

        bool isTruthy(const std::any& val);



    public:

        Environment* globals = new Environment();
        Environment* env = globals;
    
        std::any interpret(std::vector<std::unique_ptr<Stmt>>& program,Environment* e);

        // Interpreter();



};

class ReturnException : public std::runtime_error {
public:
    const std::any value;
    explicit ReturnException(std::any value);
};

#endif
