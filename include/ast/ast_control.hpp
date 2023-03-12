#ifndef LANGPROC_COMPILER_AST_CONTROL
#define LANGPROC_COMPILER_AST_CONTROL

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"
#include "util_mem.hpp"

class If: public Node {
public:
    If(Node* expression, Node* truebranch) {
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
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // give condition reg
        std::string cond_symbol = m.add_symbol("if_cond", false);
        std::string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }
        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);
        // set up branch
        std::string endi_lbl = make_label("if_endif");
        os << "\tbeq " << cond_reg << ", zero, " << endi_lbl << std::endl;
        // compile statements
        this->stats[0]->compile(os, dest, m);
        // end label
        os << endi_lbl << ":" << std::endl;
    }
};


class IfElse: public Node {
public:
    IfElse(Node* expression, Node* truebranch, Node* falsebranch) {
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
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // give condition reg
        std::string cond_symbol = m.add_symbol("ifelse_cond", false);
        std::string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }
        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);
        // set up branch
        std::string else_lbl = make_label("ifelse_else");
        std::string endi_lbl = make_label("ifelse_endif");
        os << "\tbeq " << cond_reg << ", zero, " << else_lbl << std::endl;
        // compile true statement
        this->stats[0]->compile(os, dest, m);
        os << "\tbeq zero, zero, " << endi_lbl << std::endl;
        // add false label
        os << else_lbl<<  ":" << std::endl;
        // compile false statement
        this->stats[1]->compile(os, dest, m);
        // add end label
        os << endi_lbl << ":" <<  std::endl;
    }
};


class While: public Node {
public:
    While(Node* expression, Node* branch) {
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
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // give condition reg
        std::string cond_symbol = m.add_symbol("ifelse_cond", false);
        std::string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }
        // make labels
        std::string strt_lbl = make_label("while_start");
        std::string endl_lbl = make_label("while_end");
        // add while start label
        os << strt_lbl << ":" <<  std::endl;
        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);
        // set up branch
        os << "\tbeq " << cond_reg << ", zero, " << endl_lbl << std::endl;
        // compile statements
        this->stats[0]->compile(os, dest, m);
        // jump unconditionally to start
        os << "\tbeq zero, zero, " << strt_lbl << std::endl;
        // add end label
        os <<endl_lbl << ":" <<  std::endl;
    }
};

#endif
