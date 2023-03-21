#ifndef compiler_ast_operators_binary_arithmetic_sub
#define compiler_ast_operators_binary_arithmetic_sub

#include "ast.hpp"
using namespace std;

class SubOp: public Node{
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

#endif
