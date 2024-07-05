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
};

// struct Stmt{
//     NodeType type;

// };

class Expr {
public:
    NodeType type;
    // other members...

    // Ensure there is a virtual destructor if using polymorphism
    virtual ~Expr() = default;
};

class Program{
public:
    NodeType type = NodeType::PROGRAM;
    std::vector<Expr> body;

    Program(std::vector<Expr> body) : body(body) {}
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
#endif 