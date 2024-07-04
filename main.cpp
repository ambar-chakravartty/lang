#include <iostream>
#include <string>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "include/ast.hpp"

//using std::string;


void printExpr(const Expr& expr);

// Function to print a program
void printProgram(const Program& program) {
    std::cout << "Program: " << std::endl;
    for (const auto& expr : program.body) {
        printExpr(expr);
    }
}

// Function to print a binary expression
void printBinaryExpr(const BinaryExpr& binaryExpr) {
    std::cout << "BinaryExpr: " << std::endl;
    std::cout << "Left: ";
    printExpr(binaryExpr.left);
    std::cout << "Operator: " << binaryExpr.op << std::endl;
    std::cout << "Right: ";
    printExpr(binaryExpr.right);
}

// Function to print an identifier
// void printIdentifier(const Identifier& identifier) {
//     std::cout << "Identifier: " << identifier.symbol << std::endl;
// }

// Function to print a numeric literal
void printNumericLiteral(const NumericLiteral& numericLiteral) {
    std::cout << "NumericLiteral: " << numericLiteral.value << std::endl;
}

// Function to print a string literal
void printStringLiteral(const StringLiteral& stringLiteral) {
    std::cout << "StringLiteral: " << stringLiteral.value << std::endl;
}

// Function to print an expression
void printExpr(const Expr& expr) {
    switch (expr.type) {
        case NodeType::BINARY_EXP:
            printBinaryExpr(static_cast<const BinaryExpr&>(expr));
            break;
        
        case NodeType::NUM_LITERAL:
            printNumericLiteral(static_cast<const NumericLiteral&>(expr));
            break;
        case NodeType::STR_LITERAL:
            printStringLiteral(static_cast<const StringLiteral&>(expr));
            break;
        default:
            std::cerr << "Unknown expression type" << std::endl;
    }
}

void repl(){
  while(1){
    std::string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    Scanner scanner(source);

    scanner.scanTokens();

    // for (const Token& token : scanner.tokens) {
    //     std::cout << "TokenType: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
    // }

    Parser p(scanner.tokens);

    Program ast = p.parse();

    // std::cout << static_cast<int>(ast.type) << "\n"; 
   
    // for(const Token& tok: scanner.tokens){
    //   //scanner.printToken(tok);
    //   std::cout << tok.value << "\n";
    // }

    printProgram(ast);
    
   
  }
}

int main(int argc,char** argv){
  
  repl();
  return 0;
}
