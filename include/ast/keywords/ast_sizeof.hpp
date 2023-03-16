#ifndef compiler_ast_keywords_sizeof
#define compiler_ast_keywords_sizeof

#include "ast.hpp"
using namespace std;

// sizeof (exprssion)
class SizeOf: public Node {
    // Constructors
    SizeOf(Node* expression): Node{"sizeof", "", "", expression, nullptr} {}
    
    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "sizeof ";
        this->exprs[0]->print(os, "");
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        if (this->exprs[0]->get_type() != "char") {
            os << "\tli " << dest << ", 4\n"; // anything is a size of 4 bytes
        } else {
            os << "\tli " << dest << ", 1\n"; // except char
        }
    }
}

#endif
