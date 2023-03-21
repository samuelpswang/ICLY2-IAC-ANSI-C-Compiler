#ifndef compiler_ast_node
#define compiler_ast_node

#include "ast.hpp"

class Node {
public:
    // Constructors
    Node() {
        this->name = "";
        this->type = "";
        this->val = "";
        this->exprs = { };
        this->stats = { };
    }
    Node(const std::string& name, const std::string& type, const std::string& \ 
        val, Node* expr, Node* stat) {
        this->name = name;
        this->type = type;
        this->val = val;
        if (expr) this->exprs = { expr };
        else this->exprs = { };
        if (stat) this->stats = { stat };
        else this->stats = { };
    }
    // Destructors
    virtual ~Node() {}

    // Getters
    std::string get_type() const { return this->type; }
    std::string get_name() const { return this->name; }
    std::string get_val() const { return this->val; }
    std::vector<Node*> get_expr_list() const { return this->exprs; }
    std::vector<Node*> get_stat_list() const { return this->stats; }
    Node* get_expr(int i) const { return this->exprs[i]; }
    Node* get_stat(int i) const { return this->stats[i]; }
    int get_expr_size() const { return this->exprs.size(); }
    int get_stat_size() const { return this->stats.size(); }

    // Accessors
    void append_expr(Node* expr) { exprs.push_back(expr); }
    void append_statement(Node* expr) { stats.push_back(expr); }
    
    // Members 
    virtual void print(std::ostream& os, const std::string& indent) const {};
    virtual void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {};
    
protected:
    std::string type; // type of node
    std::string name; // name, e.g. "int var = 3" -> var
    std::string val; // value, e.g. "int var = 3" -> 3
    std::vector<Node*> exprs; // expression list, whatever is in ()
    std::vector<Node*> stats; // statements list, whatever is in {} 
};

#endif
