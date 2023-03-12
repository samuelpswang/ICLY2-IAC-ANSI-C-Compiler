#ifndef LANGPROC_COMPILER_AST_EXPRESSION
#define LANGPROC_COMPILER_AST_EXPRESSION

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"
#include "util_mem.hpp"


class Declaration: public Node {
public:
    Declaration(const std::string& type,const std::string& name) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        std::string iden_name_in_symtable = m.add_symbol(this->name, true);
        std::string iden_reg = m.asm_give_reg(os, iden_name_in_symtable, treg);
        if (iden_reg == "") {
            m.asm_spill_all(os, treg);
            iden_reg = m.asm_give_reg(os, iden_name_in_symtable, treg);
        }
        m.asm_store_symbol(os, iden_name_in_symtable);
    }
};


class InitDeclaration: public Node {
public:
    InitDeclaration(const std::string& type, const std::string& name,Node* expr) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = { expr };
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name << " = ";
        this->exprs[0]->print(os, "");
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // setting iden 0
        std::string iden_name_in_symtable = m.add_symbol(this->name, true);
        std::string iden_reg = m.asm_load_symbol(os, iden_name_in_symtable, treg);
        if (iden_reg == "") {
            m.asm_spill_all(os, treg);
            iden_reg = m.asm_load_symbol(os, iden_name_in_symtable, treg);
        }
        // calculating express
        this->exprs[0]->compile(os, iden_reg, m);
        // store value back to memory
        m.asm_store_symbol(os, iden_name_in_symtable);
    }
};


#endif
