#ifndef compiler_ast_types_enum
#define compiler_ast_types_enum

#include "ast.hpp"
using namespace std;

// enum ... { item_name, ... }
class EnumItem: public Node {
public:
    // Constructor
    EnumItem(string item_name): Node{"enumitem", item_name, "", nullptr, \
        nullptr} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->name;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        m.add_en_symbol(this->name, false, 0);
        m.add_type(this->name, "enum", 4);
    }
};

// enum ... { item_name = item value, ... }
class EnumItemWithValue: public Node {
public:
    // Constructor
    EnumItemWithValue(string item_name, string item_value):
        Node{"enumitemwithvalue", item_name, item_value, nullptr, nullptr} {}
        
    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << this->name << " = " << this->val;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        m.add_en_symbol(this->name, true, stoi(this->val));
        m.add_type(this->name, "enum", 4);
    }
};

// enum (enum_name) { (enum_item), ... }
// * use add_expr for multiple enum items
class EnumDeclarator: public Node {
public:
    // Constructor
    EnumDeclarator(string enum_name, Node* enum_item): Node{"enumdeclarator", \
        enum_name, "", enum_item, nullptr} {}
        
    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "enum " << this->name << "{ ";
        for (size_t i = 0; i < this->exprs.size(); i++) {
            if (i != this->exprs.size()-1) {
                this->exprs[i]->print(os, "");
                os << ", ";
            } else {
                this->exprs[i]->print(os, "");
            }
        }
        os << " };\n";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        for (auto& e: this->exprs) {
            e->compile(os, dest, m);
        }
        m.add_type(this->name, "enum", 4);
    }
};


class EnumList: public Node {
public:
    EnumList(Node* argument) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {argument};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            if(i != this->exprs.size()-1){
                os<<",";
            }
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }    
    }
    
};

#endif
