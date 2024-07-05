#include <iostream>
#include <memory>
#include <string>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "include/ast.hpp"


void printType(std::unique_ptr<Expr>& root);
void printType(Expr* e);

void printNum(NumericLiteral* n){
    std::cout << n->value << "\n";
}

void printStrLit(StringLiteral* s){
    std::cout << s->value << "\n";
}

void printBinary(BinaryExpr* b){
     printType(b->left.get());            
     std::cout << b->op << "\n";
     printType(b->right.get());
}

void printType(Expr* e){
    switch (e->type) {
        case NodeType::NUM_LITERAL:
            printNum(static_cast<NumericLiteral*>(e));           
            break;  
        case NodeType::STR_LITERAL:
            printStrLit(static_cast<StringLiteral*>(e));
            break;
        case NodeType::BINARY_EXP:
            printBinary(static_cast<BinaryExpr*>(e));           
            break;
        
        default:
            std::cerr << "Unknown expression type\n";
    }
}

void printType(std::unique_ptr<Expr>& root){
    printType(root.get());
}

void repl(){
  while(1){
    std::string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    Scanner scanner(source);

    scanner.scanTokens();

    Parser p(scanner.tokens);

    auto r = p.parse(); 

    printType(r);
   
  }
}

int main(int argc,char** argv){
  
  repl();
  return 0;
}
