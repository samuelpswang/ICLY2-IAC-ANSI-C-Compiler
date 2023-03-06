#ifndef LANGPROC_COMPILER_AST_OPERATOR
#define LANGPROC_COMPILER_AST_OPERATOR

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class AssignOp: public Node {
public:
    AssignOp(const Node* expr, const Node* statement) {
        this->type = "";
        this->name = "";
        this->val = "";
        this->exprs = { expr };
        this->stats = { statement };
    }
    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        this->exprs[0]->print(os, "");
        os << " = ";
        this->stats[0]->print(os, "");
    }
    void compile(std::ostream& os, const std::string& dest) const {
        this->exprs[0]->compile(os, dest);
        this->stats[0]->compile(os, dest);
    }
};


class PostfixUnaryIncDecOp : public Node {
public:
    PostfixUnaryIncDecOp(const std::string& incdecop, const Node* unary_expression) {
        this->type = "";
        this->name = incdecop;
        this->val = "";
        this->exprs = { unary_expression };
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        this->exprs[0]->print(os,"");
        os << this->name;
    }

    void compile(std::ostream& os, const std::string& dest) const {
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
    }
};


class PrefixUnaryIncDecOp : public Node {
public:
    PrefixUnaryIncDecOp(const std::string& incdecop, const Node* unary_expression) {
        this->type = "";
        this->name = incdecop;
        this->val = "";
        this->exprs = { unary_expression };
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent) const {
        os << indent;
        os << this->name;
        this->exprs[0]->print(os,"");
        
    }

    void compile(std::ostream& os, const std::string& dest) const {
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
    }
};


class MultOp : public Node{
public:
    MultOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "*" ;
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        // TODO:
    }
};

class DivOp : public Node{
public:
    DivOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "/";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        // TODO:
    }

};

class ModOp : public Node {
public:
    ModOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "%";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"multop: Not implemented"<<std::endl;
    }

};

class AddOp : public Node{
public:
    AddOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "+";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        std::string left_reg;
    }
};

class SubOp : public Node{
public:
    SubOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "-";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }
    void compile(std::ostream& os, const std::string& dest) const{
        os<<"subop: Not implemented"<<std::endl;
    }
};

class LeftShift : public Node{

public:
    LeftShift(const Node* left, const Node* right){
        this->type = "";
        this->name = "<<";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"leftshiftop: Not implemented"<<std::endl;
    }

};

class RightShift : public Node{

public:
    RightShift(const Node* left, const Node* right){
        this->type = "";
        this->name = ">>";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"rightshiftop: Not implemented"<<std::endl;
    }

};
class LessThanOp : public Node{

public:
    LessThanOp(const Node* left, const Node* right){
        this->type = "";
        this->name = "<";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"lessthanop: Not implemented"<<std::endl;
    }

};

class MoreThanOp : public Node{

public:
    MoreThanOp(const Node* left, const Node* right){
        this->type = "";
        this->name = ">";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"morethanop: Not implemented"<<std::endl;
    }

};

class LessEqual : public Node{

public:
    LessEqual(const Node* left, const Node* right){
        this->type = "";
        this->name = "<=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"lessequalop: Not implemented"<<std::endl;
    }

};

class MoreEqual : public Node{

public:
    MoreEqual(const Node* left, const Node* right){
        this->type = "";
        this->name = ">=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"moreequalop: Not implemented"<<std::endl;
    }

};

class EqualTo : public Node{

public:
    EqualTo(const Node* left, const Node* right){
        this->type = "";
        this->name = "==";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"moreequalop: Not implemented"<<std::endl;
    }

};

class NotEqualTo : public Node{

public:
    NotEqualTo(const Node* left, const Node* right){
        this->type = "";
        this->name = "!=";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"moreequalop: Not implemented"<<std::endl;
    }

};

class BitwiseAnd : public Node{

public:
    BitwiseAnd(const Node* left, const Node* right){
        this->type = "";
        this->name = "&";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"bitwiseandop: Not implemented"<<std::endl;
    }
};

class BitwiseXor : public Node{

public:
    BitwiseXor(const Node* left, const Node* right){
        this->type = "";
        this->name = "^";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"bitwisexorop: Not implemented"<<std::endl;
    }
};

class BitwiseOr : public Node{

public:
    BitwiseOr(const Node* left, const Node* right){
        this->type = "";
        this->name = "|";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"bitwiseorop: Not implemented"<<std::endl;
    }
};

class LogicalAnd : public Node{

public:
    LogicalAnd(const Node* left, const Node* right){
        this->type = "";
        this->name = "&&";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"logicaland: Not implemented"<<std::endl;
    }
};

class LogicalOr : public Node{

public:
    LogicalOr(const Node* left, const Node* right){
        this->type = "";
        this->name = "||";
        this->val = "";
        this->exprs = { left, right };
        this->stats = {};
    }
    
    void print(std::ostream& os, const std::string& indent) const{
        os<<indent<<"(";
        this->exprs[0]->print(os,"");
        os<<this->name;
        this->exprs[1]->print(os,"");
        os<<")";
    }

    void compile(std::ostream& os, const std::string& dest) const{
        os<<"logicalor: Not implemented"<<std::endl;
    }
};

class PrefixPointerOp : public Node{

public:
    PrefixPointerOp(const Node* expr){
        this->type = "";
        this->name = "*";
        this->val = "";
        this->exprs = {expr};
        this->stats = {};
    }

    void print(std::ostream& os, const std::string& indent)const {
        os<< indent << this->name;
        this->exprs[0]->print(os,"");
    }

    void compile(std::ostream& os, const std::string& dst)const{
        os<<"prefixpointerop: not implemented"<<std::endl;
    }
};

class NegOp : public Node{

public:
    NegOp(const Node* expr){
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

    void compile(std::ostream& os, const std::string& dst)const{
        os<<"negpointerop: not implemented"<<std::endl;
    }
};







#endif
