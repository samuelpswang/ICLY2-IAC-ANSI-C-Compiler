#ifndef LANGPROC_COMPILER_AST
#define LANGPROC_COMPILER_AST

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "ast/ast_control.hpp"
#include "ast/ast_expression.hpp"
#include "ast/ast_function.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_operator.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_value.hpp"
#include "ast/ast_root.hpp"

extern const Node* parse_ast();

// class Tree {
// public:
//     Tree() {}
//     ~Tree() {}
//     void print(std::ostream& os) const {
//         main->print(os, "s");
//     }
    
//     void compiile(std::ostream& os) const {
//         os << "not implemented";
//     }

// private:
//     Node* main;
//     std::map<std::string, Node*> symbols;
// };

#endif
