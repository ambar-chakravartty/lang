#include "include/Environment.hpp"
#include "include/Values.hpp"
#include <cstdarg>
#include <iostream>
#include <memory>
#include <utility>


void Environment::define(std::string name, std::unique_ptr<RuntimeVal> val) {
    auto it = values.find(name);
    if (it != values.end()) {
       std::cout << "Variable already exists\n" ;
    } else {
        switch(val->type){
            case ValueType::NUMBER:{
                auto v = static_cast<NumberVal*>(val.get());
                values[name] = std::make_unique<NumberVal>(v->value); 
                }
                break;
            case ValueType::STRVAL:{
                auto s = *static_cast<StringValue*>(val.get());
                values[name] = std::make_unique<StringValue>(s.value);
            }	break;
        }		
    }
}

void Environment::assign(std::string name, std::unique_ptr<RuntimeVal> val) {
    auto it = values.find(name);
    if (it != values.end()) {
       switch(val->type){
            case ValueType::NUMBER:{
                auto v = static_cast<NumberVal*>(val.get());
                values[name] = std::make_unique<NumberVal>(v->value); 
                }
                break;
            case ValueType::STRVAL:{
                auto s = *static_cast<StringValue*>(val.get());
                values[name] = std::make_unique<StringValue>(s.value);
            }	break;
        }	
    } else {
        	if(enclosing != nullptr){
			enclosing->assign(name,std::move(val));
		}
	
    }

}

std::unique_ptr<RuntimeVal> Environment::get(std::string name) {
    auto it = values.find(name);
    if (it != values.end()) {
       auto v = it->second.get();
        switch(v->type){
            case ValueType::NUMBER:
                return std::make_unique<NumberVal>(static_cast<NumberVal*>(v)->value);
            case ValueType::STRVAL:
                return std::make_unique<StringValue>(static_cast<StringValue*>(v)->value);
        }
    } else {
	    if(enclosing != nullptr){
		    return enclosing->get(name);
	    }
    }

    //std::cout << "Variable not defined\n";
}


