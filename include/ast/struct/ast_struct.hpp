#ifndef LANGPROC_COMPILER_AST_STRUCT
#define LANGPROC_COMPILER_AST_STRUCT

#include "ast.hpp"

class StructDefinition: public Node{

public:
    StructDefinition(const std::string& struct_name, Node* struct_member_list):
        Node{"struct","struct",struct_name,struct_member_list,nullptr} {}

    void print(std::ostream& os, const std::string& indent)const override{
        os<<"struct "<<this->val<<":\n";
        for(int i = 0; i< this->exprs.size(); i++){
            os<<"\t";
            this->exprs[i]->print(os,"");
            os<<std::endl;
        }
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m)const override{
        for(int i = 0; i< this->exprs[0]->get_expr_list().size(); i++){
            m.add_struct_member(this->val,this->exprs[0]->get_expr(i)->get_name(),this->exprs[0]->get_expr(i)->get_type());
        }
    }
};


class StructInitVariable: public Node{

public:
    StructInitVariable(const std::string& struct_name, const std::string& struct_variable):
        Node{"struct",struct_name,struct_variable,nullptr,nullptr} {}

    void print(std::ostream& os, const std::string& indent)const override{
        os<<"struct "<<this->val<<":\n";
        for(int i = 0; i< this->exprs.size(); i++){
            os<<"\t";
            this->exprs[i]->print(os,"");
            os<<std::endl;
        }
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m)const override{
        m.add_struct_declaration(this->type, this->val);
    }
};


// I know this is dumb don't judge me Samuel I'm lazy


class StructMemberDeclaration: public Node {
public:
    StructMemberDeclaration(const std::string& type,const std::string& name) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {

    }
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
    void print(std::ostream& os, const std::string& indent) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->print(os,indent);
            os<<";";
            }
    }

    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        for(int i = 0; i < this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }
    }
};






#endif