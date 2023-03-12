#ifndef LANGPROC_COMPILER_AST_STATEMENT
#define LANGPROC_COMPILER_AST_STATEMENT

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"
#include "util_mem.hpp"


class Return: public Node {
public:
    Return(Node* expr) {
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
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        this->exprs[0]->compile(os, dest, m);
        os << "\tret" << std::endl;
    }
};


class Statement: public Node {
public:
    Statement(const std::string& type,Node* stat) {
        this->type = type;
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        if ((this->type) == "expression" || (this->type) == "declaration" || (this->type) == "init_declarator") {
            this->stats[0]->print(os, indent);
            os << ";" << std::endl;
        } else {
            this->stats[0]->print(os, indent);
            os<<std::endl;
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        this->stats[0]->compile(os, dest, m);
    }
};


class StatementList: public Node {
public:
    StatementList(Node* stat) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        for (auto n : this->stats) {
            n->print(os, indent);
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        for (auto n : this->stats) {
            n->compile(os, dest, m);
        }
    }
};

#endif
