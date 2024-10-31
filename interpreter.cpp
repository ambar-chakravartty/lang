#include <any>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "include/Callable.hpp"
#include "include/interpreter.hpp"
#include "include/Environment.hpp"
#include "include/ast.hpp"


ReturnException::ReturnException(std::any value) : runtime_error(""), value(std::move(value)) {

}

std::any Interpreter::evaluate(std::unique_ptr<Expr>& e,Environment* env){ //what's the difference between passing std::unique_ptr<Expr> and std::unique_ptr<Expr>& ???
    return evaluate(e.get(),env);
}


bool Interpreter::isTruthy(const std::any& val){
    if(val.type() == typeid(nullptr))
        return false;
    if(val.type() == typeid(bool))
        return std::any_cast<bool>(val);
    
    return true;
}

std::any Interpreter::evalBinary(BinaryExpr* b, Environment* e) {
    auto lhs = evaluate(b->left, e);
    auto rhs = evaluate(b->right, e);

    if (b->op == "+" || b->op == "-" || b->op == "/" || b->op == "*" || 
        b->op == "==" || b->op == ">=" || b->op == "<=" || b->op == ">" || b->op == "<") {

        if (lhs.type() != typeid(float) || rhs.type() != typeid(float)) {
            std::cerr << "Error: Operands must be of type float." << std::endl;
            exit(1);
        }

        float lhsVal = std::any_cast<float>(lhs);
        float rhsVal = std::any_cast<float>(rhs);

        if (b->op == "+") return lhsVal + rhsVal;
        if (b->op == "-") return lhsVal - rhsVal;
        if (b->op == "/") return lhsVal / rhsVal;
        if (b->op == "*") return lhsVal * rhsVal;
        if (b->op == "==") return lhsVal == rhsVal;
        if (b->op == ">=") return lhsVal >= rhsVal;
        if (b->op == "<=") return lhsVal <= rhsVal;
        if (b->op == ">") return lhsVal > rhsVal;
        if (b->op == "<") return lhsVal < rhsVal;
    }

    std::cerr << "Error: Unsupported operator '" << b->op << "'." << std::endl;
    exit(1);
}


std::any Interpreter::varExpr(Identifier* node,Environment* e) {
    std::string name = node->name;
    return e->get(name);
}

std::any Interpreter::assignment(Assignment* node,Environment* e){
	std::string name = node->name;
	auto val = evaluate(node->lhs,e);
	
	e->assign(name,val);

	return 0;

}

std::any Interpreter::executeBlock(Block* b,Environment* e){
   
    std::any lastEvaluated;
    for(std::vector<std::unique_ptr<Stmt>>::iterator i = b->statements.begin(); i != b->statements.end();++i){
        // std::cout << "stmt type  : " << static_cast<int>(i->get()->type) << "\n";
        lastEvaluated = evalStmt(i->get(),e);
    }

    return {};

   
}

std::any Interpreter::returnCall(ReturnStmt* s, Environment* e) {
    std::cout << "throw 0 \n";
    std::any value = nullptr;
    if (s->expr != nullptr) {
        value = evaluate(s->expr, e);  // Evaluate the return value
    }
    std::cout << "throw\n";
    throw ReturnException(value);
}

std::any Interpreter::call(CallExpr* c,Environment* e){
    auto callee = c->callee.get();

    if(callee->type != NodeType::IDENTIFIER){
        std::cerr << "Invalid callee\n";
        exit(EXIT_FAILURE);
    }

    auto name = static_cast<Identifier*>(callee)->name;

    
}


// std::any Interpreter::call(CallExpr* c, Environment* e) {
//     // Get the raw pointer of the Function object stored in the environment.
//     auto callee = c->callee.get();

//     if (callee->type != NodeType::IDENTIFIER) {
//         std::cerr << "Error: callee is not an Identifier." << std::endl;
//         exit(1);
//     }
//     auto name = static_cast<Identifier*>(c->callee.get())->name;

//     // Retrieve the function declaration from the environment.
//     auto decl = std::any_cast<Function>(e->get(name));

//     // Check if the number of arguments matches the function's parameter list.
//     if (c->args.size() != decl.params.size()) {
//         std::cout << c->args.size() << "\n";
//         std::cout << decl.params.size() << "\n";
//         std::cerr << "Error: Argument count mismatch for function '" << name << "'." << std::endl;
//         exit(1);
//     }

//     // Create a new environment for the function call, with the current environment as its parent.
//     auto en = new Environment(e);  // New scope for the function call

//     // Bind each parameter to its corresponding argument in the new environment.
//     for (size_t i = 0; i < decl.params.size(); ++i) {
//         en->define(decl.params[i].value, evaluate(c->args[i], e));  // Evaluate arguments and bind to parameters
//     }

//     // Check if the function body is valid.
//     if (decl.body == nullptr) {
//         std::cerr << "Error: Function body has been invalidated." << std::endl;
//         delete en;
//         exit(1);
//     }

//     // Execute the function body in the new environment.
//     try {
//         executeBlock(decl.body.get(), en);  // Run the function body
//     } catch (ReturnException& r) {
//         std::cout << "cau\n";
//         delete en;  // Clean up the environment
//         return r.value;  // Return the value caught in the exception
//     }

//     // Clean up the environment after execution.
//     // delete en;

//     return nullptr;  // If no return statement is encountered, return nullptr (or default "nil")
// }

std::any Interpreter::evaluate(Expr* node,Environment* e){
    switch(node->type){
        case NodeType::NUM_LITERAL:
            return static_cast<NumericLiteral*>(node)->value;
        case NodeType::STR_LITERAL:
            return static_cast<StringLiteral*>(node)->value;
        case NodeType::BINARY_EXP:
            return evalBinary(static_cast<BinaryExpr*>(node),e);
	case NodeType::ASGN:
	    return assignment(static_cast<Assignment*>(node),e);
        case NodeType::IDENTIFIER:
            return varExpr(static_cast<Identifier*>(node),e);
        case NodeType::CALL:
            return call(static_cast<CallExpr*>(node),e);
    }

    return nullptr;
}

void Interpreter::printStmt(std::any res,Environment* e){
  if(res.type() == typeid(std::string)){
    std::cout << std::any_cast<std::string>(res) << "\n";
  }

  if(res.type() == typeid(float)){
    std::cout << std::any_cast<float>(res) << "\n";
  }
}


std::any Interpreter::funDeclaration(FunDecl* d,Environment* e){
    auto f = std::make_any<Function>(d->name,std::move(d->params),std::move(d->body));
    e->define(d->name, f);

    return 0;
}

std::any Interpreter::evalStmt(Stmt* s,Environment* e){
    switch (s->type) {
        case NodeType::EXPR_STMT:
            return evaluate(static_cast<ExprStmt*>(s)->expr,e);
        case NodeType::PRINT_STMT:
            printStmt(evaluate(static_cast<PrintStmt*>(s)->expr,e),e);
            return nullptr;
        case NodeType::DECL:{
            std::unique_ptr<Expr> lexpr = std::move(static_cast<Declaration*>(s)->lhs);
            std::any lval = evaluate(lexpr,e);

            std::string name = static_cast<Declaration*>(s)->name;

            e->define(name, lval);

            return nullptr; 
        }
        case NodeType::BLOCK:{
            Environment en(e);
            return executeBlock(static_cast<Block*>(s),&en);

        }
        case NodeType::IF_STMT:{
           auto cond = evaluate(static_cast<IfStmt*>(s)->condition,e);

            if (isTruthy(cond)) {
               auto result = evalStmt(static_cast<IfStmt*>(s)->ifB.get(), e);
               return result;  // Propagate the return value if present
            } else if (static_cast<IfStmt*>(s)->elseB) {
               auto result = evalStmt(static_cast<IfStmt*>(s)->elseB.get(), e);
               return result;  // Propagate the return value if present
            }

            // If neither branch returns a value, return a default value.
            return nullptr;
        }break;
        case NodeType::WHILE_STMT:{
            while(isTruthy(evaluate(static_cast<WhileStmt*>(s)->condition,e))){
                evalStmt(static_cast<IfStmt*>(s)->ifB.get(),e);
            }
            return nullptr;
        }break;
        case NodeType::FUN_DECL:
            funDeclaration(static_cast<FunDecl*>(s),e);
            break;
        case NodeType::RETURN_STMT:
            returnCall(static_cast<ReturnStmt*>(s),e);
            return nullptr;
    }
    return nullptr;
}


std::any Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& program,Environment* e){

    std::any lastEvaluated ;

    for(std::vector<std::unique_ptr<Stmt>>::iterator i = program.begin(); i != program.end();++i){
        lastEvaluated = evalStmt(i->get(),e);
    }

    return lastEvaluated;
}
