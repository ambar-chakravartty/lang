#include "include/Parser.hpp"
#include "include/Scanner.hpp"
#include "include/ast.hpp"
#include <memory>
#include <string>
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
std::unique_ptr<Expr> Parser::parse(){
    while(currToken().type != TokenType::END){
        return expression();
    }
}


std::unique_ptr<Expr> Parser::expression() {
    return term();
}

std::unique_ptr<Expr> Parser::term() {
    auto left = factor();
    while (currToken().type == TokenType::STAR || currToken().type == TokenType::SLASH) {
        std::string op = eat().value;
        auto right = factor();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::factor() {
    auto left = literal();
    while (currToken().type == TokenType::PLUS || currToken().type == TokenType::MINUS) {
        std::string op = eat().value;
        auto right = literal();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::literal() {
   

    std::cout << "literal\n";
    if (currToken().type == TokenType::STRING) {
        auto s = std::make_unique<StringLiteral>("");
        s->value = eat().value;
        return s;
    } else if (currToken().type == TokenType::NUMBER) {
        auto n = std::make_unique<NumericLiteral>(0);
        n->value = std::stof(eat().value);
        return n;
    }else if(currToken().type == TokenType::LEFT_PAREN){

        eat(); // consume the parentheses
        
        std::unique_ptr<Expr> e = expression();

        if(currToken().type != TokenType::RIGHT_PAREN) std::cerr << "Missing ')' \n"; 

        eat();
        return e;

    } else {
        throw std::runtime_error("Unexpected token type in literal");
    }


}


