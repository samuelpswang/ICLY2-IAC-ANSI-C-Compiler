#ifndef compiler_ast_literals_string
#define compiler_ast_literals_string

#include "ast.hpp"
using namespace std;

class String: public Node {
public:
    // Constructors
    String(string val): Node{"string", "", val, nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "'" << this->val << "'";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {
        string start_symbol, curr_symbol;
        string temp_symbol = m.add_symbol("strtempactual", false);
        string temp_reg = m.asm_give_reg(os, temp_symbol, treg);
        if (temp_reg == "") {
            m.asm_spill_all(os, treg);
            temp_reg = m.asm_give_reg(os, temp_symbol, treg);
        }
        for (int i = 1; i < this->val.size()-1; i++) {
            curr_symbol = m.add_symbol("strtmp", false);
            if (i == 1) start_symbol = curr_symbol;
            os << "\tli " << temp_reg << ", " << int(this->val[i]) << "\n";
            os << "\tsw " << temp_reg << ", " << m.get_symbol(curr_symbol) << "(s0)\n";
        }
        os << "\taddi " << dest << ", s0, " << m.get_symbol(start_symbol) << "\n";
    }
};

#endif
