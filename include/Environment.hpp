#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP


#include <map>
#include <any>
#include <string>

class Environment{
public:
	Environment* enclosing;
	std::map<std::string,std::any> values;
        void define(const std::string& name,const std::any& val);
        void assign(const std::string& name,const std::any& val);
        std::any get(const std::string& name);

	Environment(){
		this->enclosing = nullptr;
	}

	Environment(Environment* enclosing){
		this->enclosing = enclosing;
	}
};

#endif
