#ifndef compiler_ast_operators_assignment_assign
#define compiler_ast_operators_assignment_assign

#include "ast.hpp"
using namespace std;

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

        if (this->exprs[0]->get_type() == "[]") { // if is array...cannot normal store
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
        } else if(this->exprs[0]->get_name() == "dereference") {
            // load pointer value (which is where address needs to be stored)
            os << "\tlw " << dest << ", " << m.get_symbol(this->exprs[0]->get_expr(0)->get_name()) << "(s0)\n";
            // store right value to left
            os << "\tsw " << value << ", 0(" << dest << ")\n";
        } else { // normal store
            std::string reg = m.asm_load_symbol(os, this->exprs[0]->get_name(), sreg);
            os << "\tadd " << reg <<", zero, " << value << std::endl;
            m.asm_store_symbol(os, this->exprs[0]->get_name());
        }
    }
};

#endif
