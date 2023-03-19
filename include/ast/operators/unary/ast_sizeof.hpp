#ifndef compiler_ast_operators_unary_sizeof
#define compiler_ast_operators_unary_sizeof

#include "ast.hpp"
using namespace std;

// sizeof((exprssion))
class SizeOf: public Node {

public:
    // Constructors
    // for sizeof(identifier)
    SizeOf(Node* expression): Node{"sizeof", "", "", expression, nullptr} {}
    // for sizeof(hardcoded_type)
    SizeOf(string type): Node{"sizeof", "", type, nullptr, nullptr} {}
    
    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "sizeof ";
        this->exprs[0]->print(os, "");
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        if (val != "") {
            os << "\taddi " << dest << ", zero, " << m.get_size(val)<<std::endl;
        } else {
            os << "\taddi " << dest << ", zero, " << m.get_size(this->exprs[0]->get_name())<<std::endl;
        }
        
        // TODO: add array sizeof
        // TODO: add pointers sizeof
    }
};

#endif
