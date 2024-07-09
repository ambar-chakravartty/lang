#include <memory>
#include <string>


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


std::unique_ptr<RuntimeVal> interpret(std::vector<std::unique_ptr<Expr>>& program){

    std::unique_ptr<RuntimeVal> lastEvaluated ;

    for(std::vector<std::unique_ptr<Expr>>::iterator i = program.begin(); i != program.end();++i){
        lastEvaluated = evaluate(*i);
    }

    return lastEvaluated;
}
