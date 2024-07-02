#ifndef AST_HPP
#define AST_HPP

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

class Expr{
public:
    NodeType type;
};

class Program{
public:
    NodeType type = NodeType::PROGRAM;
    std::vector<Expr> body;

    Program(std::vector<Expr> body) : body(body) {}
};



class BinaryExpr : public Expr{
public:
    NodeType type = NodeType::BINARY_EXP;
    Expr left;
    Expr right;
    std::string op;

    BinaryExpr(Expr left,std::string op,Expr right): left(left),op(op),right(right) {}
};

class Identifier : public Expr{
public:
    NodeType type = NodeType::IDENTIFIER;
    std::string symbol;
};

class NumericLiteral : public Expr{
public:
    NodeType type = NodeType::NUM_LITERAL;
    float value;

    NumericLiteral(float value) : value(value) {}
};

class StringLiteral : public Expr{
public:
    NodeType type = NodeType::STR_LITERAL;
    std::string value;

    StringLiteral(std::string value) : value(value) {}
};

#endif 