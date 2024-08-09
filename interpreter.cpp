#include <memory>
#include <string>
#include <iostream>

#include "include/interpreter.hpp"
#include "include/Environment.hpp"
#include "include/Values.hpp"
#include "include/ast.hpp"

// std::unique_ptr<RuntimeVal> evaluate(Expr* node);



std::unique_ptr<RuntimeVal> Interpreter::evaluate(std::unique_ptr<Expr>& e,Environment& env){ //what's the difference between passing std::unique_ptr<Expr> and std::unique_ptr<Expr>& ???
    return evaluate(e.get(),env);
}


std::unique_ptr<RuntimeVal> Interpreter::evalNumericBinary(NumberVal* lhs,std::string op,NumberVal* rhs,Environment& e){
    float result;

    if(op == "+") result = lhs->value + rhs->value;
    if(op == "-") result = lhs->value - rhs->value;
    if(op == "/") result = lhs->value / rhs->value;
    if(op == "*") result = lhs->value * rhs->value;

    return std::make_unique<NumberVal>(result);
    
}

std::unique_ptr<RuntimeVal> Interpreter::evalBinary(BinaryExpr* b,Environment& e){
    auto lhs = evaluate(b->left,e);
    auto rhs = evaluate(b->right,e);

    if(lhs->type == ValueType::NUMBER && rhs->type == ValueType::NUMBER){
        return evalNumericBinary(static_cast<NumberVal*>(lhs.get()),b->op,static_cast<NumberVal*>(rhs.get()),e);
    }

}

std::unique_ptr<RuntimeVal> Interpreter::varExpr(Identifier* node,Environment& e) {
    std::string name = node->name;
    return e.get(name);
}

std::unique_ptr<RuntimeVal> Interpreter::assignment(Assignment* node,Environment& e){
	std::string name = node->name;
	auto val = evaluate(node->lhs,e);
	
	e.assign(name,std::move(val));

	return std::make_unique<NumberVal> (0);

}

std::unique_ptr<RuntimeVal> Interpreter::executeBlock(Block* b,Environment& e){
   
    std::unique_ptr<RuntimeVal> lastEvaluated ;
    for(std::vector<std::unique_ptr<Stmt>>::iterator i = b->statements.begin(); i != b->statements.end();++i){
        lastEvaluated = evalStmt(i->get(),e);
    }

    return lastEvaluated;

   
}

std::unique_ptr<RuntimeVal> Interpreter::evaluate(Expr* node,Environment& e){
    switch(node->type){
        case NodeType::NUM_LITERAL:
            return std::make_unique<NumberVal>(static_cast<NumericLiteral*>(node)->value);
        case NodeType::STR_LITERAL:
            return std::make_unique<StringValue>(static_cast<StringLiteral*>(node)->value);
        case NodeType::BINARY_EXP:
            return evalBinary(static_cast<BinaryExpr*>(node),e);
	case NodeType::ASGN:
	    return assignment(static_cast<Assignment*>(node),e);
        case NodeType::IDENTIFIER:
            return varExpr(static_cast<Identifier*>(node),e);
    }

    return nullptr;
}

void Interpreter::printStmt(std::unique_ptr<RuntimeVal> res,Environment& e){
        if(res->type == ValueType::NUMBER){
        std::cout << static_cast<NumberVal*>(res.get())->value << "\n";
    }else if(res->type == ValueType::STRVAL){
        std::cout << static_cast<StringValue*>(res.get())->value << "\n";
    }
}

std::unique_ptr<RuntimeVal> Interpreter::evalStmt(Stmt* s,Environment& e){
    switch (s->type) {
        case NodeType::EXPR_STMT:
            return evaluate(static_cast<ExprStmt*>(s)->expr,e);
        case NodeType::PRINT_STMT:
            printStmt(evaluate(static_cast<PrintStmt*>(s)->expr,e),e);
            return std::make_unique<NumberVal>(0);
        case NodeType::DECL:{
            std::unique_ptr<Expr> lexpr = std::move(static_cast<Declaration*>(s)->lhs);
            std::unique_ptr<RuntimeVal> lval = evaluate(lexpr,e);

            std::string name = static_cast<Declaration*>(s)->name;

            e.define(name, std::move(lval));

            return std::make_unique<NumberVal>(0); }
        case NodeType::BLOCK:{
            Environment en;
            return executeBlock(static_cast<Block*>(s),en);

        }
        }

    return nullptr;
}


std::unique_ptr<RuntimeVal> Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& program,Environment& e){

    std::unique_ptr<RuntimeVal> lastEvaluated ;

    for(std::vector<std::unique_ptr<Stmt>>::iterator i = program.begin(); i != program.end();++i){
        lastEvaluated = evalStmt(i->get(),e);
    }

    return lastEvaluated;
}
