#include <iostream>
#include <string>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "include/ast.hpp"

void repl(){
  while(1){
    std::string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    Scanner scanner(source);

    scanner.scanTokens();

    Parser p(scanner.tokens);

    std::cout << static_cast<int>(p.parse().type )<< "\n";    
   
  }
}

int main(int argc,char** argv){
  
  repl();
  return 0;
}
