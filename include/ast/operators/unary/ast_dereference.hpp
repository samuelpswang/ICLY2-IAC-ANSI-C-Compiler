#ifndef compiler_ast_operators_unary_dereference
#define compiler_ast_operators_unary_dereference

#include "ast.hpp"
using namespace std;

class DereferenceOperator: public Node {
public:
    // Constructor
    DereferenceOperator(Node* expression): \
        Node{"dereference", "", "", expression, nullptr} {}

    // Members
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "*";
        this->exprs[0]->print(os, "");
    };
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        string symbol_name = this->exprs[0]->get_name();
        string variable_reg = m.asm_load_symbol(os, this->exprs[0]->get_name(), treg);
        if (variable_reg == "") {
            m.asm_spill_all(os, treg);
            variable_reg = m.asm_load_symbol(os, this->exprs[0]->get_name(), treg);
        }
        os << "\tadd " << dest << ", " << variable_reg << ", zero\n";
    };
};

#endif
