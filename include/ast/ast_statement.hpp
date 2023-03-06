#ifndef LANGPROC_COMPILER_AST_STATEMENT
#define LANGPROC_COMPILER_AST_STATEMENT

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class Return: public Node {
public:
    Return(const Node* expr) {
        this->type = "return";
        this->name = "";
        this->val = "";
        this->exprs = { expr };
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent << "return ";
        this->exprs[0]->print(os, "");
        os << ";";
    }
    void compile(std::ostream& os, const std::string& dest) const {
        this->exprs[0]->compile(os, dest);
    }
};

class Statement: public Node {
public:
    Statement(const std::string& type,const Node* stat) {
        this->type = type;
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        if((this->type) == "expression" || (this->type) == "declaration" || (this->type) == "init_declarator"){
            this->stats[0]->print(os, indent);
            os << ";" << std::endl;
        }

        else{
            this->stats[0]->print(os, indent);
            os<<std::endl;
        }
    }
    void compile(std::ostream& os, const std::string& dest) const {
        this->stats[0]->compile(os, dest);
    }
};

class StatementList: public Node {
public:
    StatementList(const Node* stat) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    StatementList(const Node* list, const Node* stat) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { list, stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        if (this->stats.size() == 1) {
            this->stats[0]->print(os, indent);
        } else {
            this->stats[0]->print(os, indent);
            this->stats[1]->print(os, indent);
        }
    }
    void compile(std::ostream& os, const std::string& dest) const {
        if (this->stats.size() == 1) {
            this->stats[0]->compile(os, dest);
        } else {
            this->stats[0]->compile(os, dest);
            this->stats[1]->compile(os, dest);
        }
    }
};

#endif
