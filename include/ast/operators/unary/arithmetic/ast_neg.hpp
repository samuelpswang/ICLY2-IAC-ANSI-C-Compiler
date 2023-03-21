#ifndef compiler_ast_operators_unary_arithmetic_neg
#define compiler_ast_operators_unary_arithmetic_eg

#include "ast.hpp"
using namespace std;

class NegOp: public Node {
public:
    NegOp(Node* expr){
        this->type = "";
        this->name = "-";
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent)const {
        os<< indent << this->name;
        this->exprs[0]->print(os,"");
    }
    void compile(std::ostream& os, const std::string& dest, const std::string& indent)const{
        os<<"\tnegw "<<dest<<", "<<dest<<std::endl;
    }
};

#endif
