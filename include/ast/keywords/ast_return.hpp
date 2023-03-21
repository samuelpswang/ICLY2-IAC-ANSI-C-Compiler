#ifndef compiler_ast_keywords_return
#define compiler_ast_keywords_return

#include "ast.hpp"
using namespace std;

class Return: public Node {
public:
    Return(Node* expr) {
        this->type = "return";
        this->name = "";
        this->val = "";
        this->exprs = { expr };
        this->stats = {};
    }
    void print(ostream& os, const string& indent) const {
        os << indent << "return ";
        this->exprs[0]->print(os, "");
        os << ";";
    }
    void compile(ostream& os, const string& dest, MemoryContext& m) const {
        this->exprs[0]->compile(os, dest, m);
        os<<"\tlw ra, 128(sp)"<<endl;
        os<<"\tlw s0, 124(sp)"<<endl;
        os<<"\taddi sp, sp, 128"<<endl;
        os << "\tret" << endl;
    }
};

#endif
