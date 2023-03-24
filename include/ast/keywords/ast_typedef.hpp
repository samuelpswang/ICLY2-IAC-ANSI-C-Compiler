#ifndef compiler_ast_keywords_typedef
#define compiler_ast_keywords_typedef

#include "ast.hpp"
using namespace std;

class Typedef: public Node {
public:
    // Constructors
    Typedef(string type, string alias, bool is_pointer): Node{alias, type, to_string(is_pointer), nullptr, nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const {
        os << indent << "typedef " << type << " " << val << ";\n";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {
        if (this->val == "1") {
            string actual_type = m.get_td_symbol(this->type);
            m.add_td_symbol(name, actual_type+"*");
            m.add_type(this->name, actual_type+"*", 4);
        } else {
            string actual_type = m.get_td_symbol(this->type);
            m.add_td_symbol(name, actual_type);
            m.add_type(this->name, actual_type, m.get_size(actual_type));
        }
    }
};

#endif
