#ifndef compiler_ast_control_flow_while
#define compiler_ast_control_flow_while

#include "ast.hpp"
using namespace std;

// while ( ... ) { ... }
// * accomodates {}
class While: public Node {
public:
    // Constructors
    While(Node* expression, Node* statements): \
        Node{"while", "", "", expression, statements} {
        if (!statements) this->stats.push_back(new Node());
    }
    // Destructors
    ~While() {
        for (Node* p: this->stats) { if (p->get_type() == "") delete p; }
    }

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "while (";
        this->exprs[0]->print(os, "");
        os << ") {\n";
        if (this->stats[0]->get_type() == "") {
            os << " }\n";
        } else {
            this->stats[0]->print(os, indent+"\t");
            os << indent << "}\n";
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {
        // give condition reg
        string cond_symbol = m.add_symbol("while_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }

        // make labels
        string start_label = make_label("while_start");
        string end_label = make_label("while_end");

        // add while start label
        os << start_label << ":\n";

        // compile condition reg
        this->exprs[0]->compile(os, cond_reg, m);

        // set up branch
        os << "\tbeq " << cond_reg << ", zero, " << end_label << "\n";

        // compile statements
        this->stats[0]->compile(os, dest, m);

        // jump unconditionally to start
        os << "\tbeq zero, zero, " << start_label << "\n";

        // add end label
        os << end_label << ":" << "\n";
    }
};


// do { ... } while ( ... )
// * accomodates empty {}
class DoWhile: public Node {
public:
    // Constructor
    DoWhile(Node* statements, Node* expression): \
        Node{"dowhile", "", "", expression, statements} {
        if (!statements) this->stats.push_back(new Node());
    }
    // Destructor
    ~DoWhile() {
        for (Node* p: this->stats) { if (p->get_type() == "") delete p; }
    }

    // Members
    void print(ostream& os, const string& indent) const {
        os << indent << "do {";
        if (this->stats[0]->get_type() == "") {
            os << " ";
        } else {
            this->stats[0]->print(os, indent+"\t");
        }
        os << indent << "}\nwhile (";
        this->exprs[0]->print(os, indent);
        os << indent << ");";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const {
        // give condition reg
        string cond_symbol = m.add_symbol("dowhile_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }

        // make labels
        string start_label = make_label("dowhile_start");
        string cond_label = make_label("dowhile_cond");
        string end_label = make_label("dowhile_end");

        // add while start label
        os << start_label << ":\n";

        // compile statements
        this->stats[0]->compile(os, dest, m);

        // compile condition
        this->exprs[0]->compile(os, cond_reg, m);
        
        // set up branch
        os << "\tbeq " << cond_reg << ", zero, " << end_label << "\n";

        // jump unconditionally to start
        os << "\tbeq zero, zero, " << start_label << "\n";

        // add end label
        os << end_label << ":" << "\n";
    }
};

#endif
