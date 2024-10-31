
#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>
#include <string>
#include "Scanner.hpp"

enum class NodeType {
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
    virtual ~Expr() = default;

    // Deep copy constructor
    virtual std::unique_ptr<Expr> clone() const = 0;
};

class Program {
public:
    NodeType type = NodeType::PROGRAM;
    std::vector<std::unique_ptr<Expr>> body;

    Program(std::vector<std::unique_ptr<Expr>> body) : body(std::move(body)) {}

    // Deep copy constructor
    Program(const Program& other) {
        type = other.type;
        body.reserve(other.body.size());
        for (const auto& expr : other.body) {
            body.push_back(expr->clone());
        }
    }

    // Assignment operator
    Program& operator=(const Program& other) {
        if (this != &other) {
            type = other.type;
            body.clear();
            body.reserve(other.body.size());
            for (const auto& expr : other.body) {
                body.push_back(expr->clone());
            }
        }
        return *this;
    }
};

class NullLiteral : public Expr {
public:
    std::string value;

    NullLiteral() {
        type = NodeType::NULL_LITERAL;
        value = "null";
    }

    // Deep copy constructor
    NullLiteral(const NullLiteral& other) : value(other.value) {
        type = other.type;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<NullLiteral>(*this);
    }
};

class NumericLiteral : public Expr {
public:
    float value;

    NumericLiteral(float value) : value(value) {
        type = NodeType::NUM_LITERAL;
    }

    // Deep copy constructor
    NumericLiteral(const NumericLiteral& other) : value(other.value) {
        type = other.type;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<NumericLiteral>(*this);
    }
};

class StringLiteral : public Expr {
public:
    std::string value;

    StringLiteral(const std::string& value) : value(value) {
        type = NodeType::STR_LITERAL;
    }

    // Deep copy constructor
    StringLiteral(const StringLiteral& other) : value(other.value) {
        type = other.type;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<StringLiteral>(*this);
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

    // Deep copy constructor
    BinaryExpr(const BinaryExpr& other)
        : left(other.left ? other.left->clone() : nullptr),
          op(other.op),
          right(other.right ? other.right->clone() : nullptr) {
        type = other.type;
    }

    // Assignment operator
    BinaryExpr& operator=(const BinaryExpr& other) {
        if (this != &other) {
            left = other.left ? other.left->clone() : nullptr;
            op = other.op;
            right = other.right ? other.right->clone() : nullptr;
            type = other.type;
        }
        return *this;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<BinaryExpr>(*this);
    }
};

class Assignment : public Expr {
public:
    std::string name;
    std::unique_ptr<Expr> lhs;

    Assignment(std::unique_ptr<Expr> lhs, std::string name)
        : lhs(std::move(lhs)), name(name) {
        type = NodeType::ASGN;
    }

    // Deep copy constructor
    Assignment(const Assignment& other)
        : name(other.name),
          lhs(other.lhs ? other.lhs->clone() : nullptr) {
        type = other.type;
    }

    // Assignment operator
    Assignment& operator=(const Assignment& other) {
        if (this != &other) {
            name = other.name;
            lhs = other.lhs ? other.lhs->clone() : nullptr;
            type = other.type;
        }
        return *this;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<Assignment>(*this);
    }
};

class Identifier : public Expr {
public:
    std::string name;

    Identifier(std::string name) : name(name) {
        type = NodeType::IDENTIFIER;
    }

    // Deep copy constructor
    Identifier(const Identifier& other) : name(other.name) {
        type = other.type;
    }

    // Deep copy method
    std::unique_ptr<Expr> clone() const override {
        return std::make_unique<Identifier>(*this);
    }
};


class Stmt{
    public:
        NodeType type;

        virtual ~Stmt() = default;
        virtual std::unique_ptr<Stmt> clone() const = 0;
};

class PrintStmt : public Stmt{
    public:
        std::unique_ptr<Expr> expr;

        PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
            type = NodeType::PRINT_STMT;
        }

        PrintStmt(const PrintStmt& other){
            expr = other.expr->clone();
            type = other.type;
        }   

        std::unique_ptr<Stmt> clone() const override{
            return std::make_unique<PrintStmt>(*this);
        }

};

class ExprStmt : public Stmt{
    public:
        std::unique_ptr<Expr> expr;

        ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
            type = NodeType::EXPR_STMT;
        }

        ExprStmt(const ExprStmt& other){
            expr = other.expr->clone();
            type = other.type;
        }

        std::unique_ptr<Stmt> clone() const override {
            return std::make_unique<ExprStmt>(*this);
        }

};

class Declaration : public Stmt{
    public:
        std::unique_ptr<Expr> lhs;
        std::string name;

        Declaration(std::unique_ptr<Expr> lhs,std::string name) : lhs(std::move(lhs)) , name(name){
            type = NodeType::DECL;
        }

        Declaration(const Declaration& other){
            lhs = other.lhs->clone();
            name = other.name;
            type = other.type;
        }

        std::unique_ptr<Stmt> clone() const override{
            return std::make_unique<Declaration>(*this);
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

        std::unique_ptr<Stmt> clone() const override{
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

    IfStmt(const IfStmt& other) {
        condition = other.condition ? other.condition->clone() : nullptr;
        ifB = other.ifB ? other.ifB->clone() : nullptr;
        elseB = other.elseB ? other.elseB->clone() : nullptr;
        type = other.type;
    }

    std::unique_ptr<Stmt> clone() const override{
        return std::make_unique<IfStmt>(*this);
    }
};

class WhileStmt : public Stmt{
  public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> code;
    

    WhileStmt(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> code) : condition(std::move(condition)), code(std::move(code)){
        type = NodeType::WHILE_STMT;
    }  

    WhileStmt(const WhileStmt& other) {
        condition = other.condition ? other.condition->clone() : nullptr;
        code = other.code ? other.code->clone() : nullptr;
        type = other.type;

    }

    std::unique_ptr<Stmt> clone() const override{
        return std::make_unique<WhileStmt>(*this);
    }
};

class CallExpr : public Expr {
    public:
        std::unique_ptr<Expr> callee;
        std::vector<std::unique_ptr<Expr>> args;

    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args) : callee(std::move(callee)) , args(std::move(args)) {
        type = NodeType::CALL;
    }

    CallExpr(const CallExpr& other) {
        callee = other.callee ? other.callee->clone() : nullptr;

        for(const auto& a : other.args){
            args.push_back(a->clone());
        }

        type = other.type;

    }

    std::unique_ptr<Expr> clone() const override{
        return std::make_unique<CallExpr>(*this);
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

        FunDecl(const FunDecl& other) {
            name = other.name;
            body = std::make_unique<Block>(*other.body.get());

            for(auto p : other.params){
                params.push_back(p);
            }

            type = other.type;

        }

        std::unique_ptr<Stmt> clone() const override{
            return std::make_unique<FunDecl>(*this);
        }
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    ReturnStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)){
        type = NodeType::RETURN_STMT;
    }

    ReturnStmt(const ReturnStmt& other) {
        expr = other.expr ? other.expr->clone() : nullptr;
        type = other.type;

    }

    std::unique_ptr<Stmt> clone() const override{
        return std::make_unique<ReturnStmt>(*this);
    }
};
#endif 

