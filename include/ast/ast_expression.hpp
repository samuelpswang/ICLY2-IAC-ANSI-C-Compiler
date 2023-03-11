#ifndef LANGPROC_COMPILER_AST_EXPRESSION
#define LANGPROC_COMPILER_AST_EXPRESSION

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class Expr: public Node {
public:
    Expr(const std::string& type,const std::string& name) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        if(this->type != ""){
            os << indent << this->type << " " << this->name;
        }

        else{
            os << indent << this->type << this->name;
        }
    }
    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        os << "add " << dest << ", " <<this->name <<", zero"<<std::endl;
    
    }

};



class Declaration: public Node {
public:
    Declaration(const std::string& type,const std::string& name) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name;
    }
    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        os << "add " << this->name << ", zero, zero"<<std::endl;
    
    }

};


class InitDeclaration: public Node {
public:
    InitDeclaration(const std::string& type,const std::string& name,Node* expr) {
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name<<" = ";
        this->exprs[0]->print(os,"");
    }
    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        std::string assingmentReg = make_name("rs");
        this->exprs[0]->compile(os,assingmentReg,indent);
        os <<indent<< "add " << this->name <<", "<<assingmentReg<<", zero"<<std::endl;
    
    }

};


#endif
