#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>
#include <string>

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

};




// struct Stmt{
//     NodeType type;

// };

class Visitor;

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

class Visitor{
    public:
        virtual void visitNumericLiteral(NumericLiteral* node);
        virtual void visitStringLiteral(StringLiteral* node);
        virtual void visitBinaryExpression(BinaryExpr* node);

        virtual ~Visitor() = default;
};
#endif 