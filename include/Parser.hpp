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
    std::unique_ptr<Expr> parse();
    // void printExpr(Expr& e);

private:
    
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> binary();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> literal();

    Token eat();
    Token currToken();
    Token prev();


};

#endif