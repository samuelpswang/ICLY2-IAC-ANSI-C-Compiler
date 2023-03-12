#ifndef LANGPROC_COMPILER_AST_VALUE
#define LANGPROC_COMPILER_AST_VALUE

#include <iostream>
#include <string>
#include <vector>

#include "ast_node.hpp"
#include "util_mem.hpp"


class Number: public Node {
public:
    Number(std::string val) {
        this->type = "";
        this->name = "";
        this->val = val;
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->val;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        os << "\taddi " << dest << ", zero, " << this->val << std::endl;
    }
};


class Identifier: public Node {
public:
    Identifier(std::string identifier_name) {
        this->type = "";
        this->name = identifier_name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->name;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        std::string iden_reg = m.asm_load_symbol(os, this->name, treg);
        os << "\tadd" << dest << ", " << iden_reg << ", zero" << std::endl;
    }
};

#endif
