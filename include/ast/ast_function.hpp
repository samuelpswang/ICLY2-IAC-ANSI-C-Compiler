#ifndef LANGPROC_COMPILER_AST_FUNCTION
#define LANGPROC_COMPILER_AST_FUNCTION

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class Function: public Node {
public:
    Function(const std::string& type, const std::string& name, const Node* stat) { // without arguments
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name << "() {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << indent << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        this->stats[0]->compile(os, dest);
    }
};

#endif
