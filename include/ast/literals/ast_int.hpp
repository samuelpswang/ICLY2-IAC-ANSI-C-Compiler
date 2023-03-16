#ifndef compiler_ast_literals_int
#define compiler_ast_literals_int

#include "ast.hpp"
using namespace std;

// (a nunber without ".")
class Int: public Node {
public:
    // Constructors
    Int(string val): Node{"int", "", val, nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->val;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) \
        const override {
        os << "\tli " << dest << ", " << this->val << endl;
    }
};

#endif
