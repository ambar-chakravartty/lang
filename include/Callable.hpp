#include "Environment.hpp"

#include <any>
#include <memory>
#include <vector>
#include "ast.hpp"
#include "interpreter.hpp"




class Function{
    public:
        std::string name;
        std::vector<Token> params;
        std::unique_ptr<Block> body;

        Function(std::string name,std::vector<Token> params,std::unique_ptr<Block> body) : name(name), params(std::move(params)), body(std::move(body)){}

        Function(const Function& other){
            name = other.name;

            for(auto p : other.params){
                params.push_back(p);
            }

            body = std::make_unique<Block>(*other.body.get());
        }

        std::any clone() const{
            return std::make_any<Function>(*this);
    }

        void call(Interpreter& i,std::vector<std::unique_ptr<Expr>> args){

        }
      
};