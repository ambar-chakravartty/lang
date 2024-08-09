#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Values.hpp"
#include <memory>
#include <map>
class Environment{
public:
	Environment* enclosing;
	std::map<std::string,std::unique_ptr<RuntimeVal>> values;
        void define(std::string name,std::unique_ptr<RuntimeVal> val);
        void assign(std::string name,std::unique_ptr<RuntimeVal> val);
        std::unique_ptr<RuntimeVal> get(std::string name);

	Environment(){
		this->enclosing = nullptr;
	}

	Environment(Environment* enclosing){
		this->enclosing = enclosing;
	}
};

#endif
