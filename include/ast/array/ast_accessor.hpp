#ifndef compiler_ast_array_accessor
#define compiler_ast_array_accessor

#include "ast.hpp"
using namespace std;

// (name)[(index_expression)]
class ArrayAccessor: public Node {
public:
    // Constructors
    // name: name of array, e.g. arr
    // type: array type, []
    ArrayAccessor(string name, Node* index_expression): \
        Node{name, "[]", "", index_expression, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->name << "[";
        this->exprs[0]->print(os, "");
        os << "]";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        // figure out what offset is
        // load symbol from a[offset]
        
        // give register for address offset
        string offset_symbol = m.add_symbol("arrayaccessor_offset", false);
        string offset_reg = m.asm_give_reg(os, offset_symbol, treg);
        if (offset_reg == "") {
            m.asm_spill_all(os, treg);
            offset_reg = m.asm_give_reg(os, offset_symbol, treg);
        }

        // compile address offset
        this->exprs[0]->compile(os, offset_reg, m);

        // add stack offset to offset register
        os << "\tadd " << offset_reg << ", " << offset_reg << ", sp\n";

        // load word from offset
        os << "\tlw " << dest << ", " << m.get_symbol(this->name+"[0]") << "(" \
            << offset_reg << ")\n";
    }
};

#endif
