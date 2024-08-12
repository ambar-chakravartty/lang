#include "include/Parser.hpp"
#include "include/Scanner.hpp"
#include "include/ast.hpp"
#include <algorithm>
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

void Parser::parse(){
    while(currToken().type != TokenType::END){
        program.push_back(declaration());
    }
}

std::unique_ptr<Stmt> Parser::declaration(){
    if(currToken().type == TokenType::LET){
        eat();
        return varDeclaration();
    }else{
        return statement();
    }
}

std::unique_ptr<Stmt> Parser::varDeclaration(){

    std::string name = eat().value;

    std::unique_ptr<Expr> init = nullptr;

    if(currToken().type == TokenType::EQUAL){
        eat();
        init = expression();
    }

    if(currToken().type != TokenType::SEMICOLON){
        std::cerr << "Expected ';'\n";
    }

    eat(); // eat the semicolon

    return std::make_unique<Declaration>(std::move(init),name);

}

std::unique_ptr<Stmt> Parser::statement(){
    if(currToken().type == TokenType::PRINT){
        eat();
        return printStatement();
    }
   if(currToken().type == TokenType::LEFT_BRACE){
	eat(); // eat the '{'
	return std::make_unique<Block>(block());
   }

   if(currToken().type == TokenType::IF){
    eat();
    return ifStatement();
   }

   if(currToken().type == TokenType::WHILE){
    eat();
    return WhileStatement();
   }

    return exprStatement();
}

std::unique_ptr<Stmt> Parser::ifStatement(){
    //process the condition
    if(currToken().type != TokenType::LEFT_PAREN){
        std::cout << "Expect '(' after if token\n";
    }
    eat(); // consume the '('
    auto condition = expression();

    if (currToken().type != TokenType::RIGHT_PAREN) {
        std::cout << "Expect closing ')'\n";
    }
    eat(); //consume the closing paren

    auto truthy = statement();
    std::unique_ptr<Stmt> elseB = nullptr;

    if(currToken().type == TokenType::ELSE){
        eat();
        elseB = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition),std::move(truthy),std::move(elseB));

} 

std::unique_ptr<Stmt> Parser::WhileStatement(){
    //process the condition
    if(currToken().type != TokenType::LEFT_PAREN){
        std::cout << "Expect '(' after if token\n";
    }
    eat(); // consume the '('
    auto condition = expression();

    if (currToken().type != TokenType::RIGHT_PAREN) {
        std::cout << "Expect closing ')'\n";
    }
    eat(); //consume the closing paren

    auto code = statement();
   

    return std::make_unique<WhileStmt>(std::move(condition),std::move(code));

} 

std::vector<std::unique_ptr<Stmt>> Parser::block(){
    std::vector<std::unique_ptr<Stmt>> list;

    while(currToken().type != TokenType::RIGHT_BRACE && currToken().type != TokenType::END){
        list.push_back(declaration());
    }

    if(currToken().type != TokenType::RIGHT_BRACE){
        std::cout << "Expected '}' after end of block \n";
    }

    eat(); //consume the '}'

    return list;
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
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment(){
   auto expr = equality();	
     if(currToken().type == TokenType::EQUAL){
         auto name = prev().value;
	 eat(); //consume the '=' sign
         auto value = assignment();
	 expr =  std::make_unique<Assignment>(std::move(value),name);				 
      } 
    return expr;
}

std::unique_ptr<Expr> Parser::equality(){
    auto left = comparision();
    while(currToken().type == TokenType::EQUAL_EQUAL){
        std::string op = eat().value;
        auto right = comparision();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::comparision(){
    auto left = factor();
    while(currToken().type == TokenType::GREATER_EQUAL || currToken().type == TokenType::GREATER || currToken().type == TokenType::LESS || currToken().type == TokenType::LESS_EQUAL){
        std::string op = eat().value;
        auto right = factor();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }

    return left;
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

    }else if(currToken().type == TokenType::IDENTIFIER){
        return std::make_unique<Identifier>(eat().value);
    }else{
        //std::cout << "unidentified character\n";
        eat();
        return std::make_unique<NullLiteral>();
    }


}



