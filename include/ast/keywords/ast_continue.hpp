#ifndef compiler_ast_keywords_continue
#define compiler_ast_keywords_continue

#include "ast.hpp"
using namespace std;

// continue;
class Continue: public Node {
public:
    // Constructors
    Continue(): Node{"continue", "", "", nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "continue";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        os << "\tbeq zero, zero, " << m.get_cf_label().first << "\n";
    }
};

#endif
