#ifndef LANGPROC_COMPILER_AST_CONTROL
#define LANGPROC_COMPILER_AST_CONTROL

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class If: public Node {
public:
    If(const Node* expression, const Node* truebranch) {
        this->type = "if";
        this->name = "";
        this->val = "";
        this->exprs = { expression };
        this->stats = { truebranch};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "if (";
        this->exprs[0]->print(os, "");
        os << ") {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << indent << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "if: not implemented" << std::endl;
    }
};

class IfElse: public Node {
public:
    IfElse(const Node* expression, const Node* truebranch, const Node* falsebranch) {
        this->type = "ifelse";
        this->name = "";
        this->val = "";
        this->exprs = { expression };
        this->stats = { truebranch, falsebranch };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "if (";
        this->exprs[0]->print(os, "");
        os << ") {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << indent << "} else {" << std::endl;
        this->stats[1]->print(os, indent+"\t");
        os << indent << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "ifelse: not implemented" << std::endl;
    }
};

class While: public Node {
public:
    While(const Node* expression, const Node* branch) {
        this->type = "while";
        this->name = "";
        this->val = "";
        this->exprs = { expression };
        this->stats = { branch };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "while (";
        this->exprs[0]->print(os, "");
        os << ") {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        os << "while: not implemented" << std::endl;
    }
};

#endif
