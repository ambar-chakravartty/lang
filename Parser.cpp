#include "include/Parser.hpp"
#include "include/Scanner.hpp"
#include "include/ast.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

Parser::Parser(std::vector<Token> tokenList) : tokens(tokenList), current(0) {}

Token Parser::eat() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }
    return tokens.at(current++);
}

Token Parser::prev() {
    return tokens.at(current - 1);
}

Token Parser::currToken() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens");
    }
    return tokens.at(current);
}

// Program Parser::parse() {
//     std::vector<Expr> expressions;
//     while (currToken().type != TokenType::END) {
//         expressions.push_back(expression());
//     }
//     return Program(expressions);
// }
Expr Parser::parse(){
    while(currToken().type != TokenType::END){
        return expression();
    }
}


Expr Parser::expression() {
    std::cout << "expression\n";
    return term();
}

Expr Parser::term() {
    std::cout << "term\n";
    Expr left = factor();
    while (currToken().type == TokenType::STAR || currToken().type == TokenType::SLASH) {
        std::string op = eat().value;
        Expr right = factor();
        left = BinaryExpr(left, op, right);
    }
    return left;
}

Expr Parser::factor() {
    std::cout << "factor\n";
    Expr left = literal();
    while (currToken().type == TokenType::PLUS || currToken().type == TokenType::MINUS) {
        std::string op = eat().value;
        Expr right = literal();
        left = BinaryExpr(left, op, right);
    }
    return left;
}

Expr Parser::literal() {
    std::cout << "literal\n";
    if (currToken().type == TokenType::STRING) {
        return StringLiteral(eat().value);
    } else if (currToken().type == TokenType::NUMBER) {
        return NumericLiteral(std::stof(eat().value));
    } else {
        throw std::runtime_error("Unexpected token type in literal");
    }
}

// void printExpr(Expr& e) {
//     switch (e.type) {
//         case NodeType::NUM_LITERAL:
//             std::cout << "numeric literal\n";
//             break;
//         case NodeType::STR_LITERAL:
//             std::cout << "string literal\n";
//             break;
//         case NodeType::BINARY_EXP:
//             std::cout << "binary expression\n";
//             break;
//         default:
//             std::cerr << "Unknown expression type\n";
//     }
// }
