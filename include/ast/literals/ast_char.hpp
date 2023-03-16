#ifndef compiler_ast_literals_char
#define compiler_ast_literals_char

#include "ast.hpp"
using namespace std;

// (a character in single quotes)
class Char: public Node {
public:
    // Constructors
    Char(string val): Node{"char", "", val, nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "'" << this->val << "'";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) \
        const override {
        os << "\tli " << dest << ", " << int(this->val[0]) << "\n";
    }
};

#endif
