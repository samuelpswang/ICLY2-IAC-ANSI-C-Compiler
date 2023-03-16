#ifndef compiler_ast_control_flows_if
#define compiler_ast_control_flows_if

#include "ast.hpp"
using namespace std;

// if ( ... ) { ... }
// * accomodates {}
class If: public Node {
public:
    // Constructors
    If(Node* expression, Node* true_statements): \
        Node{"if", "", "", expression, true_statements} {
        if (!true_statements) this->stats.push_back(new Node());
    }
    // Destructors
    ~If() {
        for (Node* p: this->stats) { if (p->get_type() == "") delete p; }
    }
    
    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "if (";
        this->exprs[0]->print(os, "");
        os << ") {\n";
        if (this->stats[0]->get_type() == "") {
            os << " }\n";
        } else {
            this->stats[0]->print(os, indent+"\t");
            os << indent << "}\n";
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        // give condition reg
        string cond_symbol = m.add_symbol("if_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }

        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);

        // set up branch
        string end_label = make_label("if_endif");
        os << "\tbeq " << cond_reg << ", zero, " << end_label << "\n";

        // compile statements
        this->stats[0]->compile(os, dest, m);

        // end label
        os << end_label << ":" << "\n";
    }
};


// if ( ... ) { ... } else { ... }
// * accomodates {}
class IfElse: public Node {
public:
    // Constructors
    IfElse(Node* expression, Node* true_statements, Node* false_statements): \
        Node{"ifelse", "", "", expression, nullptr} {
        if (true_statements) this->stats.push_back(true_statements);
        else this->stats.push_back(new Node());
        if (false_statements) this->stats.push_back(false_statements);
        else this->stats.push_back(new Node());
    }
    // Destructors
    ~IfElse() {
        for (Node* p: this->stats) { if (p->get_type() == "") delete p; }
    }

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "if (";
        this->exprs[0]->print(os, "");
        os << ") {";
        if (this->stats[0]->get_type() == "") {
            os << " }";
        } else {
            os << "\n";
            this->stats[0]->print(os, indent+"\t");
            os << "}\n";
        }
        os << indent << " else {\n";
        if (this->stats[1]->get_type() == "") {
            os << " }";
        } else {
            os << "\n";
            this->stats[1]->print(os, indent+"\t");
            os << "}\n";
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const \
        override {
        // give condition reg
        string cond_symbol = m.add_symbol("ifelse_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }

        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);

        // set up branch
        string else_label = make_label("ifelse_else");
        string end_label = make_label("ifelse_endif");
        os << "\tbeq " << cond_reg << ", zero, " << else_label << "\n";

        // compile true statement
        this->stats[0]->compile(os, dest, m);
        os << "\tbeq zero, zero, " << end_label << "\n";

        // add false label
        os << else_label<<  ":\n";

        // compile false statement
        this->stats[1]->compile(os, dest, m);

        // add end label
        os << end_label << ":\n";
    }
};

#endif
