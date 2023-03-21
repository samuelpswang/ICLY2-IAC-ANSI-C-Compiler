#ifndef compiler_ast_operators_unary_arithmetic_prefix_inc
#define compiler_ast_operators_unary_arithmetic_prefix_inc

#include "ast.hpp"
using namespace std;

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

#endif
