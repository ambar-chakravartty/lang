#include <iostream>
#include <string>

#include "./include/Scanner.hpp"

using std::string;

void repl(){
  while(1){
    string source;
    std::cout << "> ";
    std::getline(std::cin,source);

    Scanner scanner(source);

    scanner.scanTokens();

    for(const Token& tok: scanner.tokens){
      //scanner.printToken(tok);
      std::cout << tok.value << "\n";
    }
    
   
  }
}


int main(int argc,char** argv){

  repl();
  return 0;
}
