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
        m.add_type(this->name, this->type, m.get_size(this->type));
    }
};

class DeclarationList: public Node {
public:
    DeclarationList(Node* declaration) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {declaration};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            if(i != this->exprs.size()-1){
                os<<",";
            }
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }
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
        m.add_type(this->name, this->type, m.get_size(this->type));
    }
};


class ArgumentList: public Node {
public:
    ArgumentList(Node* argument) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {argument};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            if(i != this->exprs.size()-1){
                os<<",";
            }
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }       
    }
    
};


class Argument: public Node {
public:
    Argument(const std::string& type,const std::string& name) {
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
        std::string iden_reg = m.asm_give_reg(os, iden_name_in_symtable, areg);
        if (iden_reg == "") {
            m.asm_spill_all(os, areg);
            iden_reg = m.asm_give_reg(os, iden_name_in_symtable, areg);
        }
        
        std::string current_function = m.get_curr_function();
        int offset = m.get_curr_offset(current_function)-4;
        os<<"\tsw "<<iden_reg<<", "<<offset<<"(s0)"<<std::endl;
    }
};


class PrimaryExpressionList: public Node{

public:
    PrimaryExpressionList(Node* expr){
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent) const override {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            if(i != this->exprs.size()-1){
                os<<",";
            }
        }
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const override {
        for(int i = 0; i < this->exprs.size(); i++){
            std::string iden_name_in_symtable = m.add_symbol("function_call_temp", false);
            std::string iden_reg = m.asm_give_reg(os, iden_name_in_symtable, areg);
            if (iden_reg == "") {
                m.asm_spill_all(os, areg);
                iden_reg = m.asm_give_reg(os, iden_name_in_symtable, areg);
            }
            this->exprs[i]->compile(os,iden_reg,m);
        }
    }
};



#endif
