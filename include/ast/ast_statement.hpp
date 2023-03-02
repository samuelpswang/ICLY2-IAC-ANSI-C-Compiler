#ifndef LANGPROC_COMPILER_AST_STATEMENT
#define LANGPROC_COMPILER_AST_STATEMENT

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class Statement: public Node {
public:
    Statement(const Node* stat) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }
    void print(std::ostream& os, const std::string& indent) const {
        this->stats[0]->print(os, indent);
        os << ";" << std::endl;
    }
    void compile(std::ostream& os, const std::string& indent) const {
        os << "statement: not implemented" << std::endl;
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
        os << "statementlist: not implemented";
    }
};

#endif
