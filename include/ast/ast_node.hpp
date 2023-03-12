#ifndef LANGPROC_COMPILER_AST_NODE
#define LANGPROC_COMPILER_AST_NODE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "util_mem.hpp"

static int label_unique_num = 0;

static std::string make_label(const std::string& base){
    return "." + base + std::to_string(label_unique_num++);
}

class Node {
public:
    virtual ~Node() {}
    virtual void print(std::ostream& os, const std::string& indent) const {};
    virtual void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {};
    
    void append_list(Node* expr){
        exprs.push_back(expr);
    }

    void append_statement(Node* expr){
        stats.push_back(expr);
    }

    std::string get_name() const{ 
        return name;
    }

    std::string get_val() const{
        return val;
    }

protected:
    std::string type; // type of node
    std::string name; // name, e.g. "int var = 3" -> var
    std::string val; // value, e.g. "int var = 3" -> 3
    std::vector<Node*> exprs; // expression list, whatever is in ()
    std::vector<Node*> stats; // statements list, whatever is in {} 
};

#endif
