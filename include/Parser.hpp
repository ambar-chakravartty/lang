#ifndef PARSER_HPP
#define PARSER_HPP

#include "Scanner.hpp"
#include "ast.hpp"

// #include <string>
#include <vector>

class Parser{
public:
    std::vector<Token> tokens;
    
    int current;

    Parser(std::vector<Token> tokenList);
    Program parse();
    void printExpr(Expr& e);

private:
    
    Expr expression();
    Expr binary();
    Expr term();
    Expr factor();
    Expr literal();

    Token eat();
    Token currToken();
    Token prev();


};

#endif