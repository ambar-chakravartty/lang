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
void Parser::parse(){
    while(currToken().type != TokenType::END){
        program.push_back(statement());
    }
}


std::unique_ptr<Stmt> Parser::statement(){
    if(currToken().type == TokenType::PRINT){
        eat();
        return printStatement();
    }

    return exprStatement();
}

std::unique_ptr<Stmt> Parser::printStatement(){
    auto e = expression();

    if(currToken().type != TokenType::SEMICOLON){
        std::cerr << "expected ';' \n";

    }

    eat(); //consume the semicolon
    return std::make_unique<PrintStmt>(std::move(e));
}

std::unique_ptr<Stmt> Parser::exprStatement(){
    auto e = expression();

    if(currToken().type != TokenType::SEMICOLON){
        std::cerr << "expected ';' \n";

    }

    eat(); //consume the semicolon

    return std::make_unique<ExprStmt>(std::move(e));
}

std::unique_ptr<Expr> Parser::expression() {
    return factor();
}

std::unique_ptr<Expr> Parser::factor() {
    auto left = term();
    while (currToken().type == TokenType::PLUS || currToken().type == TokenType::MINUS) {
        std::string op = eat().value;
        auto right = term();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::term() {
    auto left = literal();
    while (currToken().type == TokenType::STAR || currToken().type == TokenType::SLASH) {
        std::string op = eat().value;
        auto right = literal();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}



std::unique_ptr<Expr> Parser::literal() {
   
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
        //std::cout << "unidentified character\n";
        eat();
        return std::make_unique<NullLiteral>();
    }


}



