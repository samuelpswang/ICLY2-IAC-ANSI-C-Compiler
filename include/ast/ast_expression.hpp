#ifndef LANGPROC_COMPILER_AST_EXPRESSION
#define LANGPROC_COMPILER_AST_EXPRESSION

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class IntExpr: public Node {
public:
    IntExpr(const std::string& name) {
        this->type = "int";
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "add " << this->name << " zero zero" << std::endl;
    }
};

#endif
