#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "./include/ast.hpp"

#include "./include/interpreter.hpp"
#include "include/Values.hpp"

//TODO:
//print statements -- done
//variables (decl & assignments) -- this
//   -- declarations work (i think) can't resuse them yet, for some reason the values won't 
//loops (while)
//user defined functions
//user defined types
//basic standard library

void printType(std::unique_ptr<Expr>& root);
void printType(Expr* e);
auto exec(std::unique_ptr<Expr>& root);
auto exec(Expr* root);

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
  Interpreter i;

  while(1){
    std::string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    if(source.compare("exit") == 0){
        std::cout << "Bye\n";
        break;
    }

    //lexing

    Scanner scanner(source);

    scanner.scanTokens();

    //parsing
    Parser p(scanner.tokens);

    p.parse();

    //Execution
    auto res = i.interpret(p.program); 
   
  }

}

int main(int argc,char** argv){
  
  repl();
  
  return 0;
}
