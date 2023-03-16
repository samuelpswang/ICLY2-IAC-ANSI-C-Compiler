#ifndef compiler_ast_literals_float
#define compiler_ast_literals_float

#include "ast.hpp"
using namespace std;

// (a nunber with or without ".")
class Float: public Node {
public:
    // Constructors
    Float(string val): Node{"float", "", val, nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->val;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) \
        const override {
        // give register
        string temp_symbol = m.add_symbol("float_upper", false);
        string temp_reg = m.asm_give_reg(os, temp_symbol, treg);
        if (temp_reg == "") {
            m.asm_spill_all(os, treg);
            temp_reg = m.asm_give_reg(os, temp_symbol, treg);
        }
        
        // load
        string float_label = make_label(".LC"); // load constant
        os << "\tlui " << temp_reg << ", \%hi(" << float_label << ")\n";
        os << "\tflw " << dest << ", \%lo(" << float_label << ")(" << temp_reg \
            << ")\n";

        // directive
        os << float_label;
        os << "\t.word " << ieee754(this->val) << "\n";
    }
};

#endif
