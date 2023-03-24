#ifndef compiler_ast_types_struct
#define compiler_ast_types_struct

#include "ast.hpp"
using namespace std;


class StructDefinition: public Node{
public:
    StructDefinition(const string& struct_name, Node* struct_member_list):
        Node{"struct","struct",struct_name,struct_member_list,nullptr} {}
    void print(ostream& os, const string& indent)const override{
        os<<"struct "<<this->val<<":\n";
        for(int i = 0; i< this->exprs.size(); i++){
            os<<"\t";
            this->exprs[i]->print(os,"");
            os<<endl;
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m)const override{
        // bharathaan
        for(int i = 0; i< this->exprs[0]->get_expr_list().size(); i++){
            m.add_struct_member(this->val,this->exprs[0]->get_expr(i)->get_name(),this->exprs[0]->get_expr(i)->get_type());
        }
        // add into size table
        string struct_name = this->val;
        Node* struct_member_list_node = this->exprs[0];
        vector<Node*> struct_member_list = struct_member_list_node->get_expr_list();
        int struct_size = 0;
        for (auto& sm: struct_member_list) {
            struct_size += m.get_size(sm->get_type());
        }
        m.add_type(this->val, this->val, struct_size);
    }
};


class StructInitVariable: public Node{
public:
    StructInitVariable(const string& struct_name, const string& struct_variable):
        Node{"struct",struct_name,struct_variable,nullptr,nullptr} {}

    void print(ostream& os, const string& indent)const override{
        os<<"struct "<<this->val<<":\n";
        for(int i = 0; i< this->exprs.size(); i++){
            os<<"\t";
            this->exprs[i]->print(os,"");
            os<<endl;
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m)const override{
        // bharathaan
        m.add_struct_declaration(this->type, this->val);
        // add into size table
        m.add_type(this->val, this->type, m.get_size(this->type));
    }
};


class StructMemberDeclaration: public Node {
public:
    StructMemberDeclaration(const string& type,const string& name) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(ostream& os, const string& indent) const {
        os << indent << this->type << " " << this->name;
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const {}
};


class StructDeclarationList: public Node {
public:
    StructDeclarationList(Node* declaration) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {declaration};
        this->stats = {};
    }
    void print(ostream& os, const string& indent) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            os<<";";
            }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }
    }
};

#endif
