#ifndef LANGPROC_COMPILER_AST_OPERATOR
#define LANGPROC_COMPILER_AST_OPERATOR

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ast_node.hpp"

class AssignOp: public Node {
public:
    AssignOp(Node* expr, Node* statement) {
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
    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        std::string valueReg = make_name("rs");
        this->stats[0]->compile(os,valueReg,indent);
        os<<indent<<"add ";
        this->exprs[0]->print(os,"");
        os<<", "<<valueReg<<", zero"<<std::endl;

    }
};


class PostfixUnaryIncDecOp : public Node {
public:
    PostfixUnaryIncDecOp(const std::string& incdecop, Node* unary_expression) {
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
    }
};


class PrefixUnaryIncDecOp : public Node {
public:
    PrefixUnaryIncDecOp(const std::string& incdecop, Node* unary_expression) {
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const {
        throw std::runtime_error("ImplementationError: PostfixUnaryIncDecOp");
    }
};


class MultOp : public Node{
public:
    MultOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"mul "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }
};

class DivOp : public Node{
public:
    DivOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"div "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }

};

class ModOp : public Node {
public:
    ModOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"rem "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }

};

class AddOp : public Node{
public:
    AddOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"add "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }
};

class SubOp : public Node{
public:
    SubOp(Node* left, Node* right){
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
    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"add "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }
};

class LeftShift : public Node{

public:
    LeftShift(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"sll "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }

};

class RightShift : public Node{

public:
    RightShift(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"srl "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }

};
class LessThanOp : public Node{

public:
    LessThanOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"slt "<<dest<<", "<<rs1<<", "<<rs2<<std::endl;
    }

};

class MoreThanOp : public Node{

public:
    MoreThanOp(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string rs1 = make_name("rs");
        std::string rs2 = make_name("rs");
        std::string rs3 = make_name("rs");
        this->exprs[0]->compile(os,rs1,indent);
        this->exprs[1]->compile(os,rs2,indent);
        os<<indent<<"slt "<<rs3<<", "<<rs1<<", "<<rs2<<std::endl;
        os<<indent<<"add "<<"one, "<<"zero, "<<"0x01"<<std::endl;
        os<<indent<<"sub "<<dest<<", "<<"one, "<<rs3<<std::endl;
    }

};

class LessEqual : public Node{

public:
    LessEqual(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        std::string lessThan = make_name("rs");
        std::string left = make_name("rs");
        std::string right = make_name("rs");
        std::string diff = make_name("rs");
        std::string end =  make_label("lte_end");
        this->exprs[0]->compile(os,left,indent);
        this->exprs[1]->compile(os,right,indent);
        os<<indent<<"addi "<<diff<<", zero, 0x01"<<std::endl;
        os<<indent<<"slt "<<dest<<", "<<left<<", "<<right<<std::endl;
        os<<indent<<"beq "<<dest<<", zero, "<<diff<<std::endl;
        os<<indent<<"sub "<<diff<<", "<<left<<", "<<right<<std::endl;
        os<<indent<<"seqz "<<dest<<", "<<diff<<", "<<std::endl;
        os<<end<<":"<<std::endl;
    }

};

class MoreEqual : public Node{

public:
    MoreEqual(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        
    }

};

class EqualTo : public Node{

public:
    EqualTo(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"moreequalop: Not implemented"<<std::endl;
    }

};

class NotEqualTo : public Node{

public:
    NotEqualTo(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"moreequalop: Not implemented"<<std::endl;
    }

};

class BitwiseAnd : public Node{

public:
    BitwiseAnd(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"bitwiseandop: Not implemented"<<std::endl;
    }
};

class BitwiseXor : public Node{

public:
    BitwiseXor(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"bitwisexorop: Not implemented"<<std::endl;
    }
};

class BitwiseOr : public Node{

public:
    BitwiseOr(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"bitwiseorop: Not implemented"<<std::endl;
    }
};

class LogicalAnd : public Node{

public:
    LogicalAnd(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"logicaland: Not implemented"<<std::endl;
    }
};

class LogicalOr : public Node{

public:
    LogicalOr(Node* left, Node* right){
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

    void compile(std::ostream& os, const std::string& dest, const std::string& indent) const{
        os<<"logicalor: Not implemented"<<std::endl;
    }
};

class PrefixPointerOp : public Node{

public:
    PrefixPointerOp(Node* expr){
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

    void compile(std::ostream& os, const std::string& dst, const std::string& indent)const{
        os<<"prefixpointerop: not implemented"<<std::endl;
    }
};

class NegOp : public Node{

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

    void compile(std::ostream& os, const std::string& dst, const std::string& indent)const{
        os<<"negpointerop: not implemented"<<std::endl;
    }
};







#endif
