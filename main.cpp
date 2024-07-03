#include <iostream>
#include <string>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"

using std::string;

void repl(){
  while(1){
    string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    Scanner scanner(source);

    scanner.scanTokens();

    Parser p = Parser(scanner.tokens);

    Program ast = p.parse();

    for(Expr& e : ast.body){
      p.printExpr(e);
    }
    
    // for(const Token& tok: scanner.tokens){
    //   //scanner.printToken(tok);
    //   std::cout << tok.value << "\n";
    // }
    
   
  }
}

int main(int argc,char** argv){

  repl();
  return 0;
}
