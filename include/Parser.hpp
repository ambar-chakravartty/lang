#ifndef PARSER_HPP
#define PARSER_HPP

#include "Scanner.hpp"
#include "ast.hpp"

// #include <string>
#include <memory>
#include <vector>

class Parser{
public:
    std::vector<Token> tokens;
    int current;

    Parser(std::vector<Token> tokenList);
    void parse();
    std::vector<std::unique_ptr<Stmt>> program;
    // void printExpr(Expr& e);

private:
    
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> binary();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparision();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> literal();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> exprStatement();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> WhileStatement();
    std::unique_ptr<Expr> Call();
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> e);
    std::unique_ptr<Stmt> function();
    std::unique_ptr<Stmt> returnStatement();


    Token eat();
    Token currToken();
    Token prev();

    bool match(TokenType t);
    bool check(TokenType t);

    Token consume(TokenType t,std::string err);


};

#endif