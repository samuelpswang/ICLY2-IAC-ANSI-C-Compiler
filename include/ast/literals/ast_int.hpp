#ifndef compiler_ast_literals_int
#define compiler_ast_literals_int

#include "ast.hpp"
using namespace std;

class Int: public Node {
public:
    // Constructors
    Int(std::string val): Node{"int", "", val, nullptr, nullptr} {}

    // Members
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->val;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        os << "\tli " << dest << ", " << this->val << std::endl;
    }
};

#endif
