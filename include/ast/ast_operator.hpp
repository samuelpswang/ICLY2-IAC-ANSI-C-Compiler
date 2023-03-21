#ifndef LANGPROC_COMPILER_AST_OPERATOR
#define LANGPROC_COMPILER_AST_OPERATOR

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class AssignOp: public Node {
public:
    AssignOp(Node* expr, Node* statement) {
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
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // compile values
        std::string symbol = m.add_symbol("assign_val",false);
        std::string value = m.asm_give_reg(os, symbol, sreg);
        if (value == "") {
            m.asm_spill_all(os, sreg);
            value = m.asm_give_reg(os, symbol, sreg);
        }
        this->stats[0]->compile(os, value, m);

        // if is array...cannot normal store
        if (this->exprs[0]->get_type() == "[]") {
            int offset = m.get_symbol(this->exprs[0]->get_name()+"[0]");
            std::string array_offset_symbol = m.add_symbol("assign_array_offset",false);
            std::string array_offset_reg = m.asm_give_reg(os, array_offset_symbol, sreg);
            if (array_offset_reg == "") {
                m.asm_spill_all(os, sreg);
                array_offset_reg = m.asm_give_reg(os, symbol, sreg);
            }
            this->exprs[0]->get_expr(0)->compile(os, array_offset_reg, m);
            os << "\tneg " << array_offset_reg << ", " << array_offset_reg << "\n";
            os << "\tadd " << array_offset_reg << ", " <<  array_offset_reg << ", " << array_offset_reg << "\n";
            os << "\tadd " << array_offset_reg << ", " <<  array_offset_reg << ", " << array_offset_reg << "\n";
            os << "\tadd " << array_offset_reg << ", " <<  array_offset_reg << ", s0\n";
            os << "\tsw " << value << ", " << offset << "(" << array_offset_reg << ")\n";
        } 
        // normal store
        else if(this->exprs[0]->get_name() == "dereference"){
            
        }
        else {
            std::string reg = m.asm_load_symbol(os, this->exprs[0]->get_name(), sreg);
            os << "\tadd " << reg <<", zero, " << value << std::endl;
            m.asm_store_symbol(os, this->exprs[0]->get_name());
        }
    }
};


class PostfixUnaryIncDecOp : public Node {
public:
    PostfixUnaryIncDecOp(const std::string& incdecop, Node* unary_expression) {
        this->type = "";
        this->name = incdecop;
        this->val = "";
        this->exprs = { unary_expression };
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        this->exprs[0]->print(os,"");
        os << this->name;
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        std::string reg_file = m.asm_load_symbol(os,this->exprs[0]->get_name(),sreg);
        if(this->name == "++"){
            os<<"\tadd "<<dest<<", "<<reg_file<<", zero"<<std::endl;
            os<<"\taddi "<<reg_file<<", "<<reg_file<<", 0x01"<<std::endl;
        }
        else{
            std::string symbol1 = m.add_symbol("val1",false);
            std::string val1  = m.asm_give_reg(os,symbol1,treg);
            if(val1 == ""){
                m.asm_spill_all(os, treg);
                val1 = m.asm_give_reg(os, symbol1, treg);
            }
            os<<"\taddi "<<val1<<", zero, 0x01"<<std::endl;
            os<<"\tadd "<<dest<<", "<<reg_file<<", zero"<<std::endl;
            os<<"\tsub "<<reg_file<<", "<<reg_file<<", "<<val1<<std::endl;
        }

        m.asm_store_symbol(os,this->exprs[0]->get_name());
    }
};


class PrefixUnaryIncDecOp : public Node {
public:
    PrefixUnaryIncDecOp(const std::string& incdecop, Node* unary_expression) {
        this->type = "";
        this->name = incdecop;
        this->val = "";
        this->exprs = { unary_expression };
        this->stats = {};
    }
  
    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        os << this->name;
        this->exprs[0]->print(os,"");
        
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        std::string reg_file = m.asm_load_symbol(os,this->exprs[0]->get_val(),sreg);
        if(this->name == "++"){
            os<<"\taddi "<<reg_file<<", "<<reg_file<<", 0x01"<<std::endl;
            os<<"\tadd "<<dest<<", "<<reg_file<<", zero"<<std::endl;
        }
        else{
            std::string symbol1 = m.add_symbol("val1",false);
            std::string val1  = m.asm_give_reg(os,symbol1,treg);
            if(val1 == ""){
                m.asm_spill_all(os, treg);
                val1 = m.asm_give_reg(os, symbol1, treg);
            }
            os<<"\taddi "<<val1<<", zero, 0x01"<<std::endl;
            os<<"\tsub "<<reg_file<<", "<<reg_file<<", "<<val1<<std::endl;
            os<<"\tadd "<<dest<<", "<<reg_file<<", zero"<<std::endl;
        }
        m.asm_store_symbol(os,this->exprs[0]->get_val());
    }
};


class MultOp : public Node{
public:
    MultOp(Node* left, Node* right){
        this->type = "";
        this->name = "*" ;
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tmul "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }
};

class DivOp : public Node{
public:
    DivOp(Node* left, Node* right){
        this->type = "";
        this->name = "/";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tdiv "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class ModOp : public Node {
public:
    ModOp(Node* left, Node* right){
        this->type = "";
        this->name = "%";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\trem "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class AddOp : public Node{
public:
    AddOp(Node* left, Node* right){
        this->type = "";
        this->name = "+";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        // ask for register
        std::string right_symbol = m.add_symbol("addop_right_val", false);
        std::string right_reg  = m.asm_give_reg(os, right_symbol, treg);
        if(right_reg == "") {
            m.asm_spill_all(os, treg);
            right_reg = m.asm_give_reg(os, right_symbol, treg);
        }

        // compile left value
        this->exprs[0]->compile(os, dest, m);
        // compile right value
        this->exprs[1]->compile(os, right_reg, m);
        
        // add values together
        os << "\tadd " << dest << ", " << dest << ", " << right_reg <<std::endl;

        // clean up
        // m.asm_store_symbol(os, right_reg);
    }
};

class SubOp : public Node{
public:
    SubOp(Node* left, Node* right){
        this->type = "";
        this->name = "-";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        // ask for register
        std::string right_symbol = m.add_symbol("subop_right_val", false);
        std::string right_reg  = m.asm_give_reg(os, right_symbol, treg);
        if(right_reg == "") {
            m.asm_spill_all(os, treg);
            right_reg = m.asm_give_reg(os, right_symbol, treg);
        }
        // compile left value
        this->exprs[0]->compile(os, dest, m);
        // compile right value
        this->exprs[1]->compile(os, right_reg, m);
        
        // add values together
        os << "\tsub " << dest << ", " << dest << ", " << right_reg <<std::endl;

        // clean up
        // m.asm_store_symbol(os, right_reg);
    }
};

class LeftShift : public Node{

public:
    LeftShift(Node* left, Node* right){
        this->type = "";
        this->name = "<<";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsll "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class RightShift : public Node{

public:
    RightShift(Node* left, Node* right){
        this->type = "";
        this->name = ">>";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsrl "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};
class LessThanOp : public Node{

public:
    LessThanOp(Node* left, Node* right){
        this->type = "";
        this->name = "<";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\t"<<"slt "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class MoreThanOp : public Node{

public:
    MoreThanOp(Node* left, Node* right){
        this->type = "";
        this->name = ">";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        std::string symbol3 = m.add_symbol("val3",false);
        std::string val3  = m.asm_give_reg(os,symbol3,treg);
        if(val3 == ""){
            m.asm_spill_all(os, treg);
            val3 = m.asm_give_reg(os, symbol3, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\t"<<"slt "<<val3<<", "<<val2<<", "<<val1<<std::endl;
        os << "\tadd " << dest << ", " << val3 << ", zero" << std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
        m.asm_store_symbol(os,symbol3);
    }

};

class LessEqual : public Node{

public:
    LessEqual(Node* left, Node* right){
        this->type = "";
        this->name = "<=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string left = m.add_symbol("val2",false);
        std::string left_value  = m.asm_give_reg(os,left,treg);
        if(left_value == ""){
            m.asm_spill_all(os, treg);
            left_value = m.asm_give_reg(os, left, treg);
        }
        std::string right = m.add_symbol("val1",false);
        std::string right_value  = m.asm_give_reg(os,right,treg);
        if(right_value == ""){
            m.asm_spill_all(os, treg);
            right_value = m.asm_give_reg(os, right, treg);
        }

        this->exprs[0]->compile(os,left_value,m);
        this->exprs[1]->compile(os,right_value,m);
        os<<"\tsgt "<<dest<<", "<<left_value<<", "<<right_value<<std::endl;
        os<<"\tseqz "<<dest<<", "<<dest<<std::endl;
        m.asm_store_symbol(os,left);
        m.asm_store_symbol(os,right);
    }

};

class MoreEqual : public Node{

public:
    MoreEqual(Node* left, Node* right){
        this->type = "";
        this->name = ">=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        std::string symbol3 = m.add_symbol("val3",false);
        std::string val3  = m.asm_give_reg(os,symbol3,treg);
        if(val3 == ""){
            m.asm_spill_all(os, treg);
            val3 = m.asm_give_reg(os, symbol3, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tslt "<<val3<<", "<<val2<<", "<<val1<<std::endl;
        os << "\tseqz" << dest << ", " << val3 << std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class EqualTo : public Node{

public:
    EqualTo(Node* left, Node* right){
        this->type = "";
        this->name = "==";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsub "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        os<<"\tseqz "<<dest<<", "<<dest<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class NotEqualTo : public Node{

public:
    NotEqualTo(Node* left, Node* right){
        this->type = "";
        this->name = "!=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tslt "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        os<<"\tseqz "<<dest<<", "<<dest<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }

};

class BitwiseAnd : public Node{

public:
    BitwiseAnd(Node* left, Node* right){
        this->type = "";
        this->name = "&";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tand "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }
};

class BitwiseXor : public Node{

public:
    BitwiseXor(Node* left, Node* right){
        this->type = "";
        this->name = "^";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{

        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\txor "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
        
    }
};

class BitwiseOr : public Node{

public:
    BitwiseOr(Node* left, Node* right){
        this->type = "";
        this->name = "|";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tor "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }
};

class LogicalAnd : public Node{

public:
    LogicalAnd(Node* left, Node* right){
        this->type = "";
        this->name = "&&";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsnez "<<val1<<", "<<val1<<std::endl;
        os<<"\tsnez "<<val2<<", "<<val2<<std::endl;
        os<<"\tand "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }
};

class LogicalOr : public Node{

public:
    LogicalOr(Node* left, Node* right){
        this->type = "";
        this->name = "||";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,treg);
        if(val1 == ""){
            m.asm_spill_all(os, treg);
            val1 = m.asm_give_reg(os, symbol1, treg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,treg);
        if(val2 == ""){
            m.asm_spill_all(os, treg);
            val2 = m.asm_give_reg(os, symbol2, treg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsnez "<<val1<<", "<<val1<<std::endl;
        os<<"\tsnez "<<val2<<", "<<val2<<std::endl;
        os<<"\tor "<<dest<<", "<<val1<<", "<<val2<<std::endl;
        m.asm_store_symbol(os,symbol1);
        m.asm_store_symbol(os,symbol2);
    }
};

class NegOp : public Node{

public:
    NegOp(Node* expr){
        this->type = "";
        this->name = "-";
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent)const {
        os<< indent << this->name;
        this->exprs[0]->print(os,"");
    }

    void compile(std::ostream& os, const std::string& dest, const std::string& indent)const{
        os<<"\tnegw "<<dest<<", "<<dest<<std::endl;
    }
};







#endif
