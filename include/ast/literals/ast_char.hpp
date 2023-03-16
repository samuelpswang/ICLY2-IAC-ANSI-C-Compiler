#ifndef compiler_ast_literals_char
#define compiler_ast_literals_char

#include "ast.hpp"
using namespace std;

class Char: public Node {
public:
    // Constructors
    Char(std::string val): Node{"char", "", val, nullptr, nullptr} {}

    // Members
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->val;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        os << "\tli " << dest << ", " << int(this->val[0]) << "\n";
    }
};

#endif
