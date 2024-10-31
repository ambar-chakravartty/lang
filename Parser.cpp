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

bool Parser::match(TokenType t){
    if(currToken().type == t){
        eat();
        return true;
    }

    return false;
}

bool Parser::check(TokenType t){
    return currToken().type == t;
} 

Token Parser::consume(TokenType t,std::string err){
   if(check(t)) 
        return eat();
   else 
        std::cout << err << "\n";
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

   if(match(TokenType::FUN)){
    return function();
   }

   if(match(TokenType::RETURN)){
    return returnStatement();
   }

    return exprStatement();
}

std::unique_ptr<Stmt> Parser::returnStatement(){
    std::unique_ptr<Expr> e = nullptr;

    if(!check(TokenType::SEMICOLON)){
        e = expression();
    }

    consume(TokenType::SEMICOLON,"Expect ';' after return value");
    return std::make_unique<ReturnStmt>(std::move(e));
}

std::unique_ptr<Stmt> Parser::function(){
    std::string name = eat().value;

    consume(TokenType::LEFT_PAREN,"Expect '(' after function name");
    std::vector<Token> params;
    if(!check(TokenType::RIGHT_PAREN)){
        do{
            params.push_back(consume(TokenType::IDENTIFIER,"Expect parameter name"));
        }while(match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments");

    consume(TokenType::LEFT_BRACE,"Expect '{' before function body");

    auto body  = std::make_unique<Block>(block());

    return std::make_unique<FunDecl>(name,std::move(params),std::move(body));
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
        std::cerr << "expected ';' after print statements \n";

    }

    eat(); //consume the semicolon
    return std::make_unique<PrintStmt>(std::move(e));
}

std::unique_ptr<Stmt> Parser::exprStatement(){
    auto e = expression();

    if(currToken().type != TokenType::SEMICOLON){
        std::cerr << "expected ';' after expression\n";

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
    auto left = Call();
    while (currToken().type == TokenType::STAR || currToken().type == TokenType::SLASH) {
        std::string op = eat().value;
        auto right = Call();
        left = std::make_unique<BinaryExpr>(std::move(left),op,std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::Call(){
    auto expr = literal();

    while(true){
        if(match(TokenType::LEFT_PAREN)){
            expr = finishCall(std::move(expr));
        }else{
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> e){
    std::vector<std::unique_ptr<Expr>> args;

    if(!check(TokenType::RIGHT_PAREN)){
        do{
            if(args.size() >= 255)
                std::cerr << "Can't have more than 255 arguments \n";
            args.push_back(expression());
        }while(match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN,"Expected ')' ");

    return std::make_unique<CallExpr>(std::move(e),std::move(args));
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



