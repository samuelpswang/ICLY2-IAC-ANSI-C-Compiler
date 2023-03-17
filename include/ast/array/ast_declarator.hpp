#ifndef compiler_ast_array_declarator
#define compiler_ast_array_declarator

#include "ast.hpp"
using namespace std;

// (type) (name)[(length)]
// * do not guarenteed that everything in array is 0
class ArrayDeclarator: public Node {
    // Constructors
    // name: name of array, e.g. arr
    // type: array type, e.g. int[]
    // val: length of array, e.g. 5
    ArrayDeclarator(const string& type, const string& name, const string& len) \
        : Node(name, type+"[]", len, nullptr, nullptr) {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->type.substr(0, this->type.size()-2) << " " << \
            this->name << "[" << this->val << "];";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        // create space on stack
        int size = stoi(this->val);
        for (int i = 0; i < size; i++) {
            m.add_symbol(this->name+"["+to_string(i)+"]", true);
        }
        // add type to type table
        m.add_type(this->name, this->type, stoi(this->val) * \
            m.get_size(this->type.substr(0, this->type.size()-2)));
    }
};

#endif
