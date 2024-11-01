#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "./include/interpreter.hpp"
#include "include/Environment.hpp"
#include "include/ASTPrettyPrinter.hpp"




void repl(){
  Interpreter i;
  Environment* e = new Environment;
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
    auto res = i.interpret(p.program,e); 
   
  }

  delete e;

}

void runFile(char* filename){
	std::ifstream file;
	file.open(filename);

  if(!file){
    std::cerr << "Error opening source file\n";
  }

  std::stringstream buf;
  buf << file.rdbuf();

  auto src = buf.str();
  Environment* e = new Environment;
  auto s = new Scanner(src);

  s->scanTokens();

  Parser p(s->tokens);

  p.parse();  

  Interpreter i;

  i.globals = e;

  auto r = i.interpret(p.program,e);

  // ASTPrettyPrinter a;
  // a.print(p.program);
  delete s;
  delete e;
		
}

int main(int argc,char** argv){
  
  if(argc == 1){
	  repl();
  }else if(argc == 2){
	  runFile(argv[1]);
  }else{
	  std::cout << "Error!\n"; 
  }
  
  return 0;
}
