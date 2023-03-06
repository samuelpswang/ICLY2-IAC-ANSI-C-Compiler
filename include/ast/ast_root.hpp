#include "ast_node.hpp"


class Root: public Node{

public:
    Root(std::vector<const Node*> expr_list){
        this->type="";
        this->name="";
        this->val="";
        this->exprs= expr_list;
        this->stats = {};
    }

    void print(std::ostream& os,const std::string& indent)const{
        for(int i = 0; i<(this->exprs).size(); i++){
            (this->exprs)[i]->print(os,"");
            os<<std::endl;
        }
    }

    void compile(std::ostream& os,const std::string& dst)const{
        os<<"root: Not implemented"<<std::endl;
    }


};