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
        os << "\tneg " << offset_reg << ", " << offset_reg << "\n"; // invert: 1 -> -1
        os << "\tadd " << offset_reg << ", " << offset_reg << ", " << offset_reg << "\n"; // add once: -1 -> -2
        os << "\tadd " << offset_reg << ", " << offset_reg << ", " << offset_reg << "\n"; // add again: -2 -> -4
        os << "\tadd " << offset_reg << ", " << offset_reg << ", s0\n"; // add sp: -4 -> -4+sp

        // load word from offset
        try {
            int offset = m.get_symbol(this->name+"[0]");
            os << "\tlw " << dest << ", " << offset << "(" << offset_reg << ")\n";
        } catch (runtime_error e) {
            os << "\tlw " << dest << ", " << m.get_symbol(this->name) << "(" << offset_reg << ")\n";
            os << "\tlw " << dest << ", 0(" << dest << ")\n";
        }
    }
};

#endif
