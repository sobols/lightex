#pragma once

#include <string>
#include <list>

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace lightex {
namespace ast {

namespace x3 = boost::spirit::x3;

struct PlainText;
struct Program;
struct ProgramNode;
// struct CommandDefinition;
// struct EnvironmentDefinition;
// struct MathematicalText;
struct Command;
// struct TabularEnvironment;

struct ProgramNode : x3::variant<x3::forward_ast<PlainText>,
                                 x3::forward_ast<Command>> {
    using base_type::base_type;
    using base_type::operator=;
};

struct Program : x3::position_tagged {
  std::list<ProgramNode> nodes;
};

struct PlainText : x3::position_tagged {
  std::string text;
};

// struct CommandDefinition : x3::position_tagged {
//   string_t name;
//   int_t arguments_num;
//   Program program;
// };

// struct EnvironmentDefinition : x3::position_tagged {
//   string_t name;
//   int_t arguments_num;
//   Program pre_program;
//   Program post_program;
// };

// struct MathematicalText : x3::position_tagged {
//   string_t text;
// };

struct Command : x3::position_tagged {
  std::string name;
  std::list<Program> arguments;
};

// struct TabularEnvironment : x3::position_tagged {
//   string_t column_configuration;
//   vector_t<string_t> content;
// };

// struct Environment : x3::position_tagged {
//   string_t column_configuration;
//   vector_t<string_t> content;
// };

}  // namespace ast
}  // namespace lightex
