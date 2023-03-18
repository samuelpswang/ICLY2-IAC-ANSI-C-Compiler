#ifndef LANGPROC_COMPILER_AST
#define LANGPROC_COMPILER_AST

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "util_mem.hpp"
#include "util_float.hpp"

#include "ast/ast_node.hpp"

#include "ast/array/ast_accessor.hpp"
#include "ast/array/ast_declarator.hpp"

#include "ast/control_flows/ast_for.hpp"
#include "ast/control_flows/ast_if.hpp"
#include "ast/control_flows/ast_while.hpp"

#include "ast/literals/ast_char.hpp"
#include "ast/literals/ast_float.hpp"
#include "ast/literals/ast_int.hpp"

#include "ast/ast_operator.hpp"
#include "ast/operators/unary/ast_address_of.hpp"
#include "ast/operators/unary/ast_dereference.hpp"
#include "ast/operators/unary/ast_sizeof.hpp"

#include "ast/types/ast_enum.hpp"

#include "ast/ast_expression.hpp"
#include "ast/ast_function.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_value.hpp"
#include "ast/ast_root.hpp"

extern const Node* parse_ast();

#endif
