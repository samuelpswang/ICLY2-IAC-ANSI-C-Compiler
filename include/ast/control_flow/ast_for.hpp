#ifndef compiler_ast_control_flow_for
#define compiler_ast_control_flow_for

#include "ast.hpp"
using namespace std;

// for ( ...; ...; ) { ... }
// * accomodates () and {}
class For: public Node {
public:
    // Constructors
    For(Node* init, Node* cond, Node* final, Node* execution):
        Node{"for", "", "", nullptr, execution} {
        if (init) this->exprs.push_back(init);
        else this->exprs.push_back(new Node());
        if (cond) this->exprs.push_back(cond);
        else this->exprs.push_back(new Node());
        if (final) this->exprs.push_back(final);
        else this->exprs.push_back(new Node());
        if (!execution) this->stats.push_back(new Node());
    }
    // Destructors
    ~For() {
        for (Node* p: this->exprs) { if (p->get_type() == "") delete p; }
        for (Node* p: this->stats) { if (p->get_type() == "") delete p; }
    }

    // Members
    void print(ostream& os, const string& indent) {
        os << indent << "for (";
        for (auto it = this->exprs.begin(); it != this->exprs.end(); it++) {
            (*it)->print(os, "");
            if (it != this->exprs.end()-1) os << ";";
        }
        os << ") {";
        if (this->stats[0]->get_type() == "") {
            os << " }\n";
        } else {
            os << "\n";
            this->stats[0]->print(os, indent+"\t");
            os << indent << "}\n";
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) {
        // make labels
        string cond_label = make_label("for_cond");
        string end_label = make_label("for_end");

        // make condition register
        string cond_symbol = m.add_symbol("for_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        if (cond_reg == "") {
            m.asm_spill_all(os, treg);
            cond_reg = m.asm_give_reg(os, cond_symbol, treg);
        }

        // compile initializer statement
        this->exprs[0]->compile(os, dest, m);
        
        // add cond_label
        os << cond_label << ":\n";

        // compile condition statement
        this->exprs[1]->compile(os, cond_reg, m);

        // create branch
        os << "\tbeq " << cond_reg << ", zero, " << end_label << "\n";
        
        // compile bulk of loop
        this->stats[0]->compile(os, dest, m);
        
        // compile final statement
        this->exprs[2]->compile(os, dest, m);

        // jump unconditionally back to evaluate condition
        os << "\tbeq zero, zero, " << cond_label << "\n";

        // add end label
        os << end_label << ":\n";
    }
};

#endif
