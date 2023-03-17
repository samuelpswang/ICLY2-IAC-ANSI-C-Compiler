#ifndef compiler_ast_keywords_break
#define compiler_ast_keywords_break

#include "ast.hpp"
using namespace std;

// break;
class Break: public Node {
public:
    // Constructors
    Break(): Node{"break", "", "", nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "break";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        os << "\tbeq zero, zero, " << m.get_cf_label().second << "\n";
    }
};

#endif
