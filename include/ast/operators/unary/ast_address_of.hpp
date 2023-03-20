#ifndef compiler_ast_operators_unary_address_of
#define compiler_ast_operators_unary_address_of

#include "ast.hpp"
using namespace std;

class AddressOfOperator: public Node {
public:
    // Constructor
    AddressOfOperator(Node* expression): \
        Node{"addressof", "", "", expression, nullptr} {}

    // Members
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "&";
        this->exprs[0]->print(os, "");
    };
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        string symbol_name = this->exprs[0]->get_name();
        int stack_offset = m.get_symbol(symbol_name);
        os << "\taddi " << dest << ", s0, " << to_string(stack_offset) << "\n";
    };
};

#endif
