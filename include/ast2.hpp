#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>
#include <string>
#include "Scanner.hpp"

enum class NodeType{
    PROGRAM,
    NUM_LITERAL,
    STR_LITERAL,
    IDENTIFIER,
    BINARY_EXP,
    NULL_LITERAL,
    PRINT_STMT,
    EXPR_STMT,
    DECL,
    ASGN,
    BLOCK,
    IF_STMT,
    WHILE_STMT,
    CALL,
    FUN_DECL,
    RETURN_STMT
};

class Expr {
public:
    NodeType type;
    // other members...

    // Ensure there is a virtual destructor if using polymorphism
    virtual ~Expr() = default;
    //virtual void accept(Visitor* visitor) = 0 ;
};

class Program{
public:
    NodeType type = NodeType::PROGRAM;
    std::vector<Expr> body;

    Program(std::vector<Expr> body) : body(body) {}
};

class NullLiteral : public Expr{
    public:
        std::string value;

        NullLiteral(){
            type = NodeType::NULL_LITERAL;
            value = "null";
        }
};

class NumericLiteral : public Expr {
public:
    float value;

    NumericLiteral(float value) : value(value) {
        type = NodeType::NUM_LITERAL;
    }
};

class StringLiteral : public Expr {
public:
    std::string value;

    StringLiteral(const std::string& value) : value(value) {
        type = NodeType::STR_LITERAL;
    }
};

class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, const std::string& op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {
        type = NodeType::BINARY_EXP;
    }

};

class Assignment : public Expr{
    public:
        std::string name;
        std::unique_ptr<Expr> lhs;

        Assignment(std::unique_ptr<Expr> lhs,std::string name) : lhs(std::move(lhs)) , name(name){
            type = NodeType::ASGN;
        }
};

class Identifier : public Expr{
    public:
        std::string name;

        Identifier(std::string name) : name(name){
            type = NodeType::IDENTIFIER;
        }
};


class Stmt{
    public:
        NodeType type;

        virtual ~Stmt() = default;
        // virtual std::unique_ptr<Stmt> clone() const = 0;
};

class PrintStmt : public Stmt{
    public:
        std::unique_ptr<Expr> expr;

        PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
            type = NodeType::PRINT_STMT;
        }

};

class ExprStmt : public Stmt{
    public:
        std::unique_ptr<Expr> expr;

        ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
            type = NodeType::EXPR_STMT;
        }

};

class Declaration : public Stmt{
    public:
        std::unique_ptr<Expr> lhs;
        std::string name;

        Declaration(std::unique_ptr<Expr> lhs,std::string name) : lhs(std::move(lhs)) , name(name){
            type = NodeType::DECL;
        }
};

class Block : public Stmt{
	public:
		std::vector<std::unique_ptr<Stmt>> statements;

		Block(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)){
            type = NodeType::BLOCK;
        }	

        Block(const Block& other) {
        // Deep copy the members here
        // Assuming `statements` is a std::vector of std::unique_ptr<Statement>
        for (const auto& stmt : other.statements) {
            statements.push_back(stmt->clone()); // Assuming Statement has a clone method
        }
    }

        std::unique_ptr<Block> clone() const {
        // Assuming Block has a copy constructor
        return std::make_unique<Block>(*this);
        }

};


class IfStmt : public Stmt{
  public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> ifB;
    std::unique_ptr<Stmt> elseB;

    IfStmt(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> ifB,std::unique_ptr<Stmt> elseB) : condition(std::move(condition)), ifB(std::move(ifB)), elseB(std::move(elseB)) {
        type = NodeType::IF_STMT;
    }  
};

class WhileStmt : public Stmt{
  public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> code;
    

    WhileStmt(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> code) : condition(std::move(condition)), code(std::move(code)){
        type = NodeType::WHILE_STMT;
    }  
};

class CallExpr : public Expr {
    public:
        std::unique_ptr<Expr> callee;
        std::vector<std::unique_ptr<Expr>> args;

    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args) : callee(std::move(callee)) , args(std::move(args)) {
        type = NodeType::CALL;
    }

};


class FunDecl : public Stmt{
    public:
        std::string name;
        std::vector<Token> params;
        std::unique_ptr<Block> body;

        FunDecl(std::string name,std::vector<Token> params,std::unique_ptr<Block> body) : name(name), params(std::move(params)), body(std::move(body)){
            type = NodeType::FUN_DECL;
        }
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    ReturnStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
        type = NodeType::RETURN_STMT;
    }
};
#endif 
