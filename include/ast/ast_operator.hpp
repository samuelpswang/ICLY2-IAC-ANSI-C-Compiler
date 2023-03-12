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
        std::string symbol = m.add_symbol("val",false);
        std::string value = m.asm_give_reg(os, symbol, areg);
        if(value == ""){
            m.asm_spill_all(os, areg);
            value = m.asm_give_reg(os, symbol, areg);
        }
        this->stats[0]->compile(os,value,m);
        std::string reg = m.asm_load_symbol(os, this->exprs[0]->get_name(),areg);
        os<<"\tadd "<< reg<<", zero, "<<value<<std::endl;
        m.asm_store_symbol(os, this->exprs[0]->get_name());
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
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
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
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tmul "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tdiv "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\trem "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tadd "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string symbol1 = m.add_symbol("val1",false);
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsub "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsll "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\tsrl "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\t"<<"slt "<<dest<<", "<<val1<<", "<<val2<<std::endl;
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
        std::string val1  = m.asm_give_reg(os,symbol1,areg);
        if(val1 == ""){
            m.asm_spill_all(os, areg);
            val1 = m.asm_give_reg(os, symbol1, areg);
        }
        std::string symbol2 = m.add_symbol("val2",false);
        std::string val2  = m.asm_give_reg(os,symbol2,areg);
        if(val2 == ""){
            m.asm_spill_all(os, areg);
            val2 = m.asm_give_reg(os, symbol2, areg);
        }
        std::string symbol3 = m.add_symbol("val3",false);
        std::string val3  = m.asm_give_reg(os,symbol3,areg);
        if(val3 == ""){
            m.asm_spill_all(os, areg);
            val3 = m.asm_give_reg(os, symbol3, areg);
        }
        this->exprs[0]->compile(os,val1,m);
        this->exprs[1]->compile(os,val2,m);
        os<<"\t"<<"slt "<<val3<<", "<<val2<<", "<<val1<<std::endl;
        os << "\tadd " << dest << ", " << val3 << ", zero" << std::endl;
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
        std::string less_than = m.add_symbol("val1",false);
        std::string less_than_value  = m.asm_give_reg(os,less_than,areg);
        if(less_than == ""){
            m.asm_spill_all(os, areg);
            less_than_value = m.asm_give_reg(os, less_than, areg);
        }
        std::string left = m.add_symbol("val2",false);
        std::string left_value  = m.asm_give_reg(os,left,areg);
        if(left_value == ""){
            m.asm_spill_all(os, areg);
            left_value = m.asm_give_reg(os, left, areg);
        }
        std::string right = m.add_symbol("val1",false);
        std::string right_value  = m.asm_give_reg(os,right,areg);
        if(right_value == ""){
            m.asm_spill_all(os, areg);
            right_value = m.asm_give_reg(os, right, areg);
        }
        std::string diff = m.add_symbol("val2",false);
        std::string diff_value  = m.asm_give_reg(os,diff,areg);
        if(diff_value == ""){
            m.asm_spill_all(os, areg);
            diff_value = m.asm_give_reg(os, diff, areg);
        }
        std::string end =  make_label("lte_end");
        this->exprs[0]->compile(os,left,m);
        this->exprs[1]->compile(os,right,m);
        os<<"\taddi "<<diff<<", zero, 0x01"<<std::endl;
        os<<"\tslt "<<dest<<", "<<left<<", "<<right<<std::endl;
        os<<"\tbeq "<<dest<<", zero, "<<diff<<std::endl;
        os<<"\tsub "<<diff<<", "<<left<<", "<<right<<std::endl;
        os<<"\tseqz "<<dest<<", "<<diff<<", "<<std::endl;
        os<<end<<":"<<std::endl;
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
        os<<"moreequalop: Not implemented"<<std::endl;
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
        os<<"moreequalop: Not implemented"<<std::endl;
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
        os<<"bitwiseandop: Not implemented"<<std::endl;
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

        os<<"bitwisexorop: Not implemented"<<std::endl;
        
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
        os<<"bitwiseorop: Not implemented"<<std::endl;
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
        os<<"logicaland: Not implemented"<<std::endl;
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
        os<<"logicalor: Not implemented"<<std::endl;
    }
};

class PrefixPointerOp : public Node{

public:
    PrefixPointerOp(Node* expr){
        this->type = "";
        this->name = "*";
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent)const {
        os<< indent << this->name;
        this->exprs[0]->print(os,"");
    }

    void compile(std::ostream& os, const std::string& dst, const std::string& indent)const{
        os<<"prefixpointerop: not implemented"<<std::endl;
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

    void compile(std::ostream& os, const std::string& dst, const std::string& indent)const{
        os<<"negpointerop: not implemented"<<std::endl;
    }
};







#endif
