#ifndef compiler_ast_operators_binary_comparison_more_than
#define compiler_ast_operators_binary_comparison_more_than

#include "ast.hpp"
using namespace std;

class MoreThanOp: public Node {
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

#endif
