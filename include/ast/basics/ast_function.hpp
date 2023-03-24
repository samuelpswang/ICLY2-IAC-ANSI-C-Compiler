#ifndef compiler_ast_basics_function
#define compiler_ast_basics_function

#include "ast.hpp"
using namespace std;

class Function: public Node {
public:
    Function(const std::string& type, const std::string& name,Node* stat) { // without arguments
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {};
        this->stats = { stat };
    }

    Function(const std::string& type, const std::string& name, Node* args, Node* stat) { // with arguments
        this->type = type;
        this->name = name;
        this->val = "";
        this->exprs = {args};
        this->stats = { stat };
    }

    void print(std::ostream& os, const std::string& indent) const {
        os << indent << this->type << " " << this->name << "(";
		if(this->exprs.size() != 0){
        	this->exprs[0]->print(os,indent);
		}
        os<<"){"<<std::endl;
        this->stats[0]->print(os, indent+"\t");
        os << "}" << std::endl;
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const{
        if(this->stats[0]!=nullptr){
            os<<this->name<<":"<<std::endl;
            m.use_func(m.add_func(this->name));
            os<<"\taddi sp, sp, -128"<<std::endl;
            os<<"\tsw ra, 128(sp)"<<std::endl;
            os<<"\tsw s0, 124(sp)"<<std::endl;
            os<<"\taddi s0, sp, 128"<<std::endl;
            if (this->exprs.size() > 0 && this->exprs[0] != nullptr)
                this->exprs[0]->compile(os,dest,m);
            if (this->stats.size() > 0 && this->stats[0] != nullptr)
                this->stats[0]->compile(os, dest,m);
        }
        if (this->type == "void") {
            os<<"\tlw ra, 128(sp)"<<endl;
            os<<"\tlw s0, 124(sp)"<<endl;
            os<<"\taddi sp, sp, 128"<<endl;
            os << "\tret" << endl;
        }
    }
};


class FunctionList: public Node {
public:
    FunctionList(Node* expr) { 
        this->append_expr(expr);
    }
    void print(std::ostream& os, const std::string& indent) const {
        for(int i = 0; i< this->exprs.size(); i++){
            this->exprs[i]->print(os,"");
        }
    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
        os << ".text" << std::endl;
        os<< ".globl "<<this->exprs[this->exprs.size()-1]->get_name()<<std::endl;
        os<<std::endl;
        for(int i = 0; i< this->exprs.size(); i++){
            this->exprs[i]->compile(os,dest,m);
        }
    }
};


class FunctionCall: public Node {
public:
    FunctionCall(Node* name) { 
        this->exprs = {name};
    }

	FunctionCall(Node* name, Node* argument_list) { 
		this->exprs = {name, argument_list};
    }

    void print(std::ostream& os, const std::string& indent) const {
		if(this->exprs.size() == 2){
			os<<indent;
			this->exprs[0]->print(os,indent);
			os<<"(";
			this->exprs[1]->print(os,indent);
			os<<")";
		}

		else{
			os<<indent;
			this->exprs[0]->print(os,indent);
			os<<"()";
		}

    }
    void compile(std::ostream& os, const std::string& dest, MemoryContext& m) const {
            if(this->exprs.size() == 2){
			    this->exprs[1]->compile(os,dest,m);
		    }
			os<<"\tcall "<<this->exprs[0]->get_name()<<std::endl;
    }

};

#endif
