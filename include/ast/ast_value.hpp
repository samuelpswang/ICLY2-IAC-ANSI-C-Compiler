#ifndef LANGPROC_COMPILER_AST_VALUE
#define LANGPROC_COMPILER_AST_VALUE

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class Int: public Node {
public:
    Int(std::string val) {
        this->type = "int";
        this->name = "";
        this->val = val;
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->val;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "addi " << dest << " zero " << this->val << std::endl;
    }
};

#endif
