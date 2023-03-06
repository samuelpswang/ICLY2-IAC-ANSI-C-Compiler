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
        std::string cond_reg = make_name("if_cond");
        std::string endi_lbl = make_name("if_endif");
        this->exprs[0]->compile(os, cond_reg);
        os << "beq " << cond_reg << " zero " << endi_lbl << std::endl;
        this->stats[0]->compile(os, dest);
        os << ":" << endi_lbl << std::endl;
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
        os << indent << "if ";
        this->exprs[0]->print(os, "");
        os << " {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << indent << "} else {" << std::endl;
        this->stats[1]->print(os, indent+"\t");
        os << indent << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        std::string cond_reg = make_name("ifelse_cond");
        std::string else_lbl = make_name("ifelse_else");
        std::string endi_lbl = make_name("ifelse_endif");
        this->exprs[0]->compile(os, cond_reg);
        os << "beq " << cond_reg << " zero " << else_lbl << std::endl;
        this->stats[0]->compile(os, dest);
        os << "beq zero zero " << endi_lbl << std::endl;
        os << ":" << else_lbl << std::endl;
        this->stats[1]->compile(os, dest);
        os << ":" << endi_lbl << std::endl;
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
        os << indent << "while ";
        this->exprs[0]->print(os, "");
        os << " {" << std::endl;
        this->stats[0]->print(os, indent+"\t");
        os<<indent<< "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest) const {
        std::string cond_reg = make_name("while_cond");
        std::string strt_lbl = make_name("while_start");
        std::string endl_lbl = make_name("while_end");
        os << ":" << strt_lbl << std::endl;
        this->exprs[0]->compile(os, cond_reg);
        os << "beq " << cond_reg << " zero " << endl_lbl << std::endl;
        this->stats[0]->compile(os, dest);
        os << "beq zero zero " << strt_lbl << std::endl;
        os << ":" << endl_lbl << std::endl;
    }
};

#endif
