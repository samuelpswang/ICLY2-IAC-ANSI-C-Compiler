#ifndef LANGPROC_COMPILER_AST_CONTROL
#define LANGPROC_COMPILER_AST_CONTROL

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class IfElse: public Node {
public:
    IfElse(const Node* expression, const Node* truebranch, const Node* falsebranch) {
        this->type = "if";
        this->name = "";
        this->val = "";
        this->exprs = { expression };
        this->stats = { truebranch, falsebranch };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "if (" << this->exprs[0] << ") {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << indent << "} else {" << std::endl;
        this->stats[1]->print(os, indent+"\t");
        os << indent << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "ifelse: not implemented" << std::endl;
    }
};

#endif
