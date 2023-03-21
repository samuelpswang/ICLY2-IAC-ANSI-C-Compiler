#ifndef compiler_ast
#define compiler_ast

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>

#include "util_float.hpp"
#include "util_mem.hpp"

#include "ast/ast_node.hpp"
#include "ast/ast_root.hpp"

#include "ast/array/ast_accessor.hpp"
#include "ast/array/ast_declarator.hpp"

#include "ast/basics/ast_expression.hpp"
#include "ast/basics/ast_function.hpp"
#include "ast/basics/ast_statement.hpp"

#include "ast/control_flows/ast_for.hpp"
#include "ast/control_flows/ast_if.hpp"
#include "ast/control_flows/ast_switch.hpp"
#include "ast/control_flows/ast_while.hpp"

#include "ast/identifiers/ast_identifier.hpp"

#include "ast/keywords/ast_break.hpp"
#include "ast/keywords/ast_continue.hpp"
#include "ast/keywords/ast_return.hpp"

#include "ast/literals/ast_char.hpp"
#include "ast/literals/ast_float.hpp"
#include "ast/literals/ast_int.hpp"
#include "ast/literals/ast_string.hpp"

#include "ast/operators/assignment/ast_assign.hpp"
#include "ast/operators/assignment/ast_pointer_assign.hpp"

#include "ast/operators/binary/arithmetic/ast_add.hpp"
#include "ast/operators/binary/arithmetic/ast_div.hpp"
#include "ast/operators/binary/arithmetic/ast_mod.hpp"
#include "ast/operators/binary/arithmetic/ast_mult.hpp"
#include "ast/operators/binary/arithmetic/ast_sub.hpp"

#include "ast/operators/binary/comparison/ast_equal_to.hpp"
#include "ast/operators/binary/comparison/ast_less_equal.hpp"
#include "ast/operators/binary/comparison/ast_less_than.hpp"
#include "ast/operators/binary/comparison/ast_more_equal.hpp"
#include "ast/operators/binary/comparison/ast_more_than.hpp"
#include "ast/operators/binary/comparison/ast_not_equal_to.hpp"

#include "ast/operators/binary/logical/ast_bitwise_and.hpp"
#include "ast/operators/binary/logical/ast_bitwise_or.hpp"
#include "ast/operators/binary/logical/ast_bitwise_xor.hpp"
#include "ast/operators/binary/logical/ast_left_shift.hpp"
#include "ast/operators/binary/logical/ast_logical_and.hpp"
#include "ast/operators/binary/logical/ast_logical_or.hpp"
#include "ast/operators/binary/logical/ast_right_shift.hpp"

#include "ast/operators/unary/arithmetic/ast_neg.hpp"
#include "ast/operators/unary/arithmetic/ast_postfix_inc.hpp"
#include "ast/operators/unary/arithmetic/ast_prefix_inc.hpp"

#include "ast/operators/unary/pointer/ast_address_of.hpp"
#include "ast/operators/unary/pointer/ast_dereference.hpp"
#include "ast/operators/unary/pointer/ast_sizeof.hpp"

#include "ast/types/ast_enum.hpp"
#include "ast/types/ast_struct.hpp"

extern const Node* parse_ast();

#endif
