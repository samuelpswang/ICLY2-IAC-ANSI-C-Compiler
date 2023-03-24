#ifndef compiler_ast_operators_binary_arithmetic_add
#define compiler_ast_operators_binary_arithmetic_add

#include "ast.hpp"
using namespace std;

class AddOp: public Node{
public:
    AddOp(Node* left, Node* right){
        this->type = "";
        this->name = "+";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(ostream& os, const string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(ostream& os, const string& dest, MemoryContext& m) const {
        string ltype = m.get_type(this->exprs[0]->get_name());
        string rtype = m.get_type(this->exprs[1]->get_name());
        bool leftp = (ltype[ltype.size()-1] == '*');
        bool rightp = (rtype[rtype.size()-1] == '*');

        if (leftp && rightp) {
            throw runtime_error("AddOp: cannot add two pointers together");
        } else if (leftp && !rightp) { // pointer on left
            // ask for register
            string lefts = m.add_symbol("addop_pointermath_right", false);
            string leftr  = m.asm_give_reg(os, lefts, treg);
            if(leftr == "") {
                m.asm_spill_all(os, treg);
                leftr = m.asm_give_reg(os, lefts, treg);
            }
            
            // compile right and times 4
            this->exprs[1]->compile(os, leftr, m);
            os << "\tadd " << leftr << ", " << leftr << ", " << leftr << "\n";
            os << "\tadd " << leftr << ", " << leftr << ", " << leftr << "\n";

            // compile right
            this->exprs[0]->compile(os, dest, m);

            // add together to dest
            os << "\tadd " << dest << ", " << dest << ", " << leftr << "\n";
        } else if (rightp && !leftp) { // pointer on right
            // ask for register
            string lefts = m.add_symbol("addop_pointermath_right", false);
            string leftr  = m.asm_give_reg(os, lefts, treg);
            if(leftr == "") {
                m.asm_spill_all(os, treg);
                leftr = m.asm_give_reg(os, lefts, treg);
            }
            
            // compile left and times 4
            this->exprs[0]->compile(os, leftr, m);
            os << "\tadd " << leftr << ", " << leftr << ", " << leftr << "\n";
            os << "\tadd " << leftr << ", " << leftr << ", " << leftr << "\n";

            // compile right
            this->exprs[1]->compile(os, dest, m);

            // add together to dest
            os << "\tadd " << dest << ", " << dest << ", " << leftr << "\n";
        } else {
            // ask for register
            string right_symbol = m.add_symbol("addop_right_val", false);
            string right_reg  = m.asm_give_reg(os, right_symbol, treg);
            if(right_reg == "") {
                m.asm_spill_all(os, treg);
                right_reg = m.asm_give_reg(os, right_symbol, treg);
            }

            // compile left value
            this->exprs[0]->compile(os, dest, m);
            // compile right value
            this->exprs[1]->compile(os, right_reg, m);
            
            // add values together
            os << "\tadd " << dest << ", " << dest << ", " << right_reg <<endl;

            // clean up
            // m.asm_store_symbol(os, right_reg);
        }
    }
};

#endif
