#ifndef LANGPROC_COMPILER_AST_OPERATOR
#define LANGPROC_COMPILER_AST_OPERATOR

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class AssignOp: public Node {
public:
    AssignOp(const Node* expr, const Node* statement) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = { expr };
        this->stats = { statement };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        this->exprs[0]->print(os, "");
        os << " = ";
        this->stats[0]->print(os, "");
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "assignop: not implemented" << std::endl;
    }
};

// add arithmetic and logical operators here

#endif
