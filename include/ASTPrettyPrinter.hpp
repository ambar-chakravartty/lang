#include "ast.hpp"
#include <iostream>

class ASTPrettyPrinter {
public:
    void print(std::vector<std::unique_ptr<Stmt>>& program) {
        std::cout << "Program:" << std::endl;
        for(std::vector<std::unique_ptr<Stmt>>::iterator i = program.begin(); i != program.end();++i){
          printStmt(*i->get(),1);
        }

    }

private:
    void printExpr(const Expr& expr, int indent) {
        printIndent(indent);

        switch (expr.type) {
            case NodeType::NUM_LITERAL:
                std::cout << "NumericLiteral: " << static_cast<const NumericLiteral&>(expr).value << std::endl;
                break;

            case NodeType::STR_LITERAL:
                std::cout << "StringLiteral: \"" << static_cast<const StringLiteral&>(expr).value << "\"" << std::endl;
                break;

            case NodeType::NULL_LITERAL:
                std::cout << "NullLiteral" << std::endl;
                break;

            case NodeType::IDENTIFIER:
                std::cout << "Identifier: " << static_cast<const Identifier&>(expr).name << std::endl;
                break;

            case NodeType::BINARY_EXP:
                std::cout << "BinaryExpr: " << static_cast<const BinaryExpr&>(expr).op << std::endl;
                printExpr(*static_cast<const BinaryExpr&>(expr).left, indent + 1);
                printExpr(*static_cast<const BinaryExpr&>(expr).right, indent + 1);
                break;

            case NodeType::CALL:
                std::cout << "CallExpr:" << std::endl;
                printExpr(*static_cast<const CallExpr&>(expr).callee, indent + 1);
                for (const auto& arg : static_cast<const CallExpr&>(expr).args) {
                    printExpr(*arg, indent + 1);
                }
                break;

            case NodeType::ASGN:
                std::cout << "Assignment: " << static_cast<const Assignment&>(expr).name << std::endl;
                printExpr(*static_cast<const Assignment&>(expr).lhs, indent + 1);
                break;

            default:
                std::cout << "Unknown Expression Type" << std::endl;
                break;
        }
    }

    void printStmt(const Stmt& stmt, int indent) {
        printIndent(indent);

        switch (stmt.type) {
            case NodeType::PRINT_STMT:
                std::cout << "PrintStmt:" << std::endl;
                printExpr(*static_cast<const PrintStmt&>(stmt).expr, indent + 1);
                break;

            case NodeType::EXPR_STMT:
                std::cout << "ExprStmt:" << std::endl;
                printExpr(*static_cast<const ExprStmt&>(stmt).expr, indent + 1);
                break;

            case NodeType::DECL:
                std::cout << "Declaration: " << static_cast<const Declaration&>(stmt).name << std::endl;
                printExpr(*static_cast<const Declaration&>(stmt).lhs, indent + 1);
                break;

            case NodeType::BLOCK:
                std::cout << "Block:" << std::endl;
                for (const auto& st : static_cast<const Block&>(stmt).statements) {
                    printStmt(*st, indent + 1);
                }
                break;

            case NodeType::IF_STMT:
                std::cout << "IfStmt:" << std::endl;
                printExpr(*static_cast<const IfStmt&>(stmt).condition, indent + 1);
                printStmt(*static_cast<const IfStmt&>(stmt).ifB, indent + 1);
                if (static_cast<const IfStmt&>(stmt).elseB) {
                    printStmt(*static_cast<const IfStmt&>(stmt).elseB, indent + 1);
                }
                break;

            case NodeType::WHILE_STMT:
                std::cout << "WhileStmt:" << std::endl;
                printExpr(*static_cast<const WhileStmt&>(stmt).condition, indent + 1);
                printStmt(*static_cast<const WhileStmt&>(stmt).code, indent + 1);
                break;

            case NodeType::FUN_DECL:
                std::cout << "FunDecl: " << static_cast<const FunDecl&>(stmt).name << std::endl;
                for (const auto& param : static_cast<const FunDecl&>(stmt).params) {
                    printIndent(indent + 1);
                    std::cout << "Param: " << param.value << std::endl;
                }
                printStmt(*static_cast<const FunDecl&>(stmt).body, indent + 1);
                break;

            case NodeType::RETURN_STMT:
                std::cout << "ReturnStmt:" << std::endl;
                printExpr(*static_cast<const ReturnStmt&>(stmt).expr, indent + 1);
                break;

            default:
                std::cout << "Unknown Statement Type" << std::endl;
                break;
        }
    }

    void printIndent(int indent) {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};
