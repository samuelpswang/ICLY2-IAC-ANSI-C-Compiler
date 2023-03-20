#ifndef compiler_ast_control_flows_switch
#define compiler_ast_control_flows_switch

#include "ast.hpp"
using namespace std;

class Case: public Node {
public:
    // Constructors
    Case(Node* case_expression, Node* case_statements): Node{"case", "", "", case_expression, case_statements} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "case ";
        this->exprs[0]->print(os, "");
        os << ":\n";
        this->stats[0]->print(os, indent+"\t");
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {}
};

class CaseDefault: public Node {
public:
    // Constructors
    CaseDefault(Node* case_statements): Node{"casedefault", "", "", nullptr, case_statements} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "default:\n";
        this->stats[0]->print(os, indent+"\t");
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {}
};


class CaseList: public Node {
public:
    // Constructors
    CaseList(Node* case_node): Node{"caselist", "", "", nullptr, case_node} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        for (const auto& s: this->stats) {
            s->print(os, indent);
        }
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {}
};


class Switch: public Node {
public: 
    // Constructor
    Switch(Node* case_expression, Node* case_list): Node{"switch", "", "", case_expression, case_list} {}

    // Members
    void print(ostream& os, const string& indent) const override {
        os << indent << "switch (";
        this->exprs[0]->print(os, "");
        os << ") {\n";
        this->stats[0]->print(os, indent);
        os << "}\n";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const override {
        // push labels
        string end_label = make_label("switch_end");
        m.add_cf_label("", end_label);

        // switch condition
        string cond_symbol = m.add_symbol("switch_cond", false);
        string cond_reg = m.asm_give_reg(os, cond_symbol, sreg);
        if (cond_reg == "") {
            m.asm_spill_all(os, sreg);
            cond_reg = m.asm_give_reg(os, cond_symbol, sreg);
        }
        this->exprs[0]->compile(os, cond_reg, m);

        // make label
        int case_num = this->stats[0]->get_stat_size();
        vector<string> case_labels;
        for (int i = 0; i < case_num; i++) {
            case_labels.push_back(make_label("switch_case"));
        }

        // evaluate which to jump to
        bool has_default = false;
        for (int i = 0; i < case_num; i++) {
            // make case reg
            string case_symbol = m.add_symbol("switch_case", false);
            string case_reg = m.asm_give_reg(os, case_symbol, sreg);
            if (case_reg == "") {
                m.asm_spill_all(os, sreg);
                case_reg = m.asm_give_reg(os, cond_symbol, sreg);
            }
            // compile case condition
            Node* case_list_p = this->stats[0];
            Node* case_p = case_list_p->get_stat(i);
            if (case_p->get_name() != "casedefault") {
                has_default = true;
                case_p->get_expr(0)->compile(os, case_reg, m);
            }
            os << "\tbeq " << cond_reg << ", " << case_reg << ", " << case_labels[i] << "\n";
        }
        
        // if does not jump to condition; jump to default if it exists
        if (has_default) {
            os << "\tbeq zero, zero, " << case_labels[case_labels.size()-1] << "\n";
        } else {
            os << "\tbeq zero, zero, " << end_label << "\n";
        }
        
        // add labels and statements repeatedly
        for (int i = 0; i < case_num; i++) {
            Node* case_list_p = this->stats[0];
            Node* case_p = case_list_p->get_stat(i);

            os << case_labels[i] << ":\n";
            case_p->get_stat(0)->compile(os, dest, m);
        }

        // add end label
        os << end_label << ":\n";

        // clean house
        m.delete_cf_label();
    }
};

#endif
