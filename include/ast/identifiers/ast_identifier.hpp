#ifndef compiler_ast_identifiers_identifier
#define compiler_ast_identifiers_identifier

#include "ast.hpp"
using namespace std;

class Identifier: public Node {
public:
    // Constructors
    Identifier(string identifier_name): Node{identifier_name, "var", \
        "", nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->name;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {
        if (m.get_en_symbol_check(this->name)) {
            os << "\taddi " << dest << ", zero, " << m.get_en_symbol(this->name) << "\n";
        } else {
            string iden_reg = m.asm_load_symbol(os, this->name, treg);
            os << "\tadd " << dest << ", " << iden_reg << ", zero\n";
        }
    }
};

#endif
