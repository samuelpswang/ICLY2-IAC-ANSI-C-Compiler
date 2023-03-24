#ifndef compiler_ast_operators_unary_pointer_dereference
#define compiler_ast_operators_unary_pointer_dereference

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
        string vname = this->exprs[0]->get_name();
        int voffset = m.get_symbol(vname);
        os << "deref_b:\n";
        os << "\tlw " << dest << ", " << voffset << "(s0)\n";
        os << "\tlw " << dest << ", 0(" << dest << ")\n";
        os << "deref_e:\n";
    };
};

#endif
