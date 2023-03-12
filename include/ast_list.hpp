#ifndef ast_list_hpp
#define ast_list_hpp


#include "ast/ast_node.hpp"
#include<vector>

// Used for multiple declarations of functions
// TODO: Use this to implement multiple declaration, multiple declarator initilisation and multiple function parameters


typedef std::vector<const Node*> List;
typedef List* ListPtr;

inline ListPtr makeList(const Node* expr){
    ListPtr list = new List();
    list->push_back(expr);
    return list;
}

inline ListPtr append_list(ListPtr expr_list, const Node* expr){
    expr_list->push_back(expr);
    return expr_list;
}

#endif


