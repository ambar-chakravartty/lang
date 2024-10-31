#include "include/Environment.hpp"
#include <cstdarg>
#include <iostream>
#include <utility>


void Environment::define(const std::string& name, const std::any& val) {

   

    auto it = values.find(name);
    if (it != values.end()) {
       std::cout << "Variable already exists\n" ;
    } else {
        values[name] = val;
    }
}

void Environment::assign(const std::string& name, const std::any& val) {
    auto it = values.find(name);
    if (it != values.end()) {
       values[name] = val;
    } else {
        	if(enclosing != nullptr){
			enclosing->assign(name,std::move(val));
		}
	
    }

}



std::any Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it != values.end()) {
        // Return the value if found
        return it->second;
    } else if (enclosing != nullptr) {
        // Check the parent environment
        return enclosing->get(name);
    } else {
        // Handle the case where the variable is not found
        std::cerr << "Error: Undefined variable '" << name << "'." << std::endl;
        exit(1);
    }
}