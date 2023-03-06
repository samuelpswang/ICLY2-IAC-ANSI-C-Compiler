#ifndef LANGPROC_COMPILER_AST_NODE
#define LANGPROC_COMPILER_AST_NODE

#include <iostream>
#include <string>
#include <vector>
#include <map>

static int unique_num = -1;
static const std::string& make_name(const std::string& base) {
    return "_" + base + std::to_string(unique_num);
}

class Node {
public:
    virtual ~Node() {}
    virtual void print(std::ostream& os, const std::string& indent) const = 0;
    virtual void compile(std::ostream& os, const std::string& dest) const = 0;
protected:
    std::string type; // type of node
    std::string name; // name, e.g. "int var = 3" -> var
    std::string val; // value, e.g. "int var = 3" -> 3
    std::vector<const Node*> exprs; // expression list, whatever is in ()
    std::vector<const Node*> stats; // statements list, whatever is in {}
};

#endif
