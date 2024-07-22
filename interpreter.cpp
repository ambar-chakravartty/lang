#include <memory>
#include <string>
#include <iostream>

#include "include/interpreter.hpp"
#include "include/Values.hpp"
#include "include/ast.hpp"

std::unique_ptr<RuntimeVal> evaluate(Expr* node);

std::unique_ptr<RuntimeVal> evaluate(std::unique_ptr<Expr>& e){ //what's the difference between passing std::unique_ptr<Expr> and std::unique_ptr<Expr>& ???
    return evaluate(e.get());
}


std::unique_ptr<RuntimeVal> evalNumericBinary(NumberVal* lhs,std::string op,NumberVal* rhs){
    float result;

    if(op == "+") result = lhs->value + rhs->value;
    if(op == "-") result = lhs->value - rhs->value;
    if(op == "/") result = lhs->value / rhs->value;
    if(op == "*") result = lhs->value * rhs->value;

    return std::make_unique<NumberVal>(result);
    
}

std::unique_ptr<RuntimeVal> evalBinary(BinaryExpr* b){
    auto lhs = evaluate(b->left);
    auto rhs = evaluate(b->right);

    if(lhs->type == ValueType::NUMBER && rhs->type == ValueType::NUMBER){
        return evalNumericBinary(static_cast<NumberVal*>(lhs.get()),b->op,static_cast<NumberVal*>(rhs.get()));
    }

}

std::unique_ptr<RuntimeVal> evaluate(Expr* node){
    switch(node->type){
        case NodeType::NUM_LITERAL:
            return std::make_unique<NumberVal>(static_cast<NumericLiteral*>(node)->value);
        case NodeType::STR_LITERAL:
            return std::make_unique<StringValue>(static_cast<StringLiteral*>(node)->value);
        case NodeType::BINARY_EXP:
            return evalBinary(static_cast<BinaryExpr*>(node));
    }
}

void printStmt(std::unique_ptr<RuntimeVal> res){
        if(res->type == ValueType::NUMBER){
        std::cout << static_cast<NumberVal*>(res.get())->value << "\n";
    }else if(res->type == ValueType::STRVAL){
        std::cout << static_cast<StringValue*>(res.get())->value << "\n";
    }
}

std::unique_ptr<RuntimeVal> evalStmt(Stmt* s){
    switch (s->type) {
        case NodeType::EXPR_STMT:
            return evaluate(static_cast<ExprStmt*>(s)->expr);
        case NodeType::PRINT_STMT:
            printStmt(evaluate(static_cast<PrintStmt*>(s)->expr));
            return std::make_unique<NumberVal>(0);
        
    }
}


std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Stmt>>& program){

    std::unique_ptr<RuntimeVal> lastEvaluated ;

    for(std::vector<std::unique_ptr<Stmt>>::iterator i = program.begin(); i != program.end();++i){
        lastEvaluated = evalStmt(i->get());
    }

    return lastEvaluated;
}
