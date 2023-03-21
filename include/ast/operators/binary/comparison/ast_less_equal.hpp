#ifndef compiler_ast_operators_binary_comparison_less_equal
#define compiler_ast_operators_binary_comparison_less_equal

#include "ast.hpp"
using namespace std;

class LessEqual: public Node {
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

#endif
