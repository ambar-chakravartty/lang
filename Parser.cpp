#include "include/Parser.hpp"
#include "include/Scanner.hpp"
#include "include/ast.hpp"
#include <vector>
#include <iostream>

Parser::Parser(std::vector<Token> tokenList){
    this->tokens = tokenList;
    this->current = 0;
}

Token Parser::eat(){
    int index = current;
    current += 1;
    return tokens.at(index);
}

Token Parser::prev(){
    return tokens.at(current-1);
}

Token Parser::currToken(){
    return tokens.at(current);
}

Program Parser::parse(){
    std::vector<Expr> program;

    while(currToken().type != TokenType::END){
        program.push_back(expression());
    }

    return Program(program);
    
}

Expr Parser::expression(){
    return term();
}

Expr Parser::term(){
    Expr left = factor(); //process LHS

    while(currToken().type == TokenType::STAR || currToken().type == TokenType::SLASH){
        std::string op = eat().value; //consume the operator
        Expr right = factor(); // process RHS
        left = BinaryExpr(left,op,right); // construct new binary expression
    }
    return left;
}

Expr Parser::factor(){
    Expr left = literal(); //process LHS

    while(currToken().type == TokenType::PLUS || currToken().type == TokenType::MINUS){
        std::string op = eat().value; //consume the operator
        Expr right = literal(); // process RHS
        left = BinaryExpr(left,op,right); // construct new binary expression
    }
    return left;
}

Expr Parser::literal(){
    if(currToken().type == TokenType::STRING){
        return StringLiteral(eat().value);
    }else if(currToken().type == TokenType::NUMBER){
        return NumericLiteral(std::stof(eat().value));
    }
}

void Parser::printExpr(Expr& e){
   switch (e.type) {
        case NodeType::NUM_LITERAL:
            std::cout << "LITERAL(" << static_cast<NumericLiteral&>(e).value << ")\n";
            break;
        case NodeType::STR_LITERAL:
            std::cout << "LITERAL(" << static_cast<StringLiteral&>(e).value << ")\n";
            break;        
        case NodeType::BINARY_EXP:
            std::cout << "BINARY ( \n";
            std::cout << "LEFT : ";
            printExpr(static_cast<BinaryExpr&>(e).left);
            std::cout << "\nRIGHT : ";
            printExpr(static_cast<BinaryExpr&>(e).right);
            std::cout << "\nOPERATOR : ";
            std::cout << static_cast<BinaryExpr&>(e).op << "\n)";
            break;           
   }
}