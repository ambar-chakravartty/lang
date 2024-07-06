#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "ast.hpp"

class Visitor{
    virtual void visitNumericLiteral(NumericLiteral* n);
    virtual void visitStringLiteral(StringLiteral* s);
    virtual void visitBinary(BinaryExpr* b);

    virtual ~Visitor() = default;
};
#endif