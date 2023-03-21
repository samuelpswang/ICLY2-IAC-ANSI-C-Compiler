#ifndef compiler_ast_operators_assignment_pointer_assign
#define compiler_ast_operators_assignment_pointer_assign

#include "ast.hpp"
using namespace std;

class PointerAssignOp: public Node {
public:
    PointerAssignOp(Node* expr, Node* statement) {
        this->type = "pointerassign";
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
        std::string value = m.asm_give_reg(os, symbol, treg);
        if (value == "") {
            m.asm_spill_all(os, treg);
            value = m.asm_give_reg(os, symbol, treg);
        }
        this->stats[0]->compile(os, value, m);

        std::string pointer_var = m.add_symbol(this->exprs[0]->get_expr(0)->get_name(), true);
        std::string reg = m.asm_give_reg(os, pointer_var, treg);
        os << "\tadd " << reg <<", zero, " << value << std::endl;
        m.asm_store_symbol(os, this->exprs[0]->get_expr(0)->get_name());
        
    }
};

#endif
