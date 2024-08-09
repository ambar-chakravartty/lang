#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "./include/Scanner.hpp"
#include "./include/Parser.hpp"
#include "./include/interpreter.hpp"
#include "include/Environment.hpp"

//TODO:
//print statements -- done
//variables (decl & assignments) -- this
//   -- declarations work (i think) can't resuse them yet, for some reason the values won't 
//loops (while)
//user defined functions
//user defined types
//basic standard library


void repl(){
  Interpreter i;
  Environment e;
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
  Environment e;
  Scanner s(src);

  s.scanTokens();

  Parser p(s.tokens);

  p.parse();  


  // some driver code for debugging the frontend
  // for(std::vector<std::unique_ptr<Stmt>>::iterator i = p.program.begin(); i != p.program.end();++i){
  //   std::cout << static_cast<int>(i->get()->type) << "\n";
  // }

  Interpreter i;

  auto r = i.interpret(p.program,e);
		
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
