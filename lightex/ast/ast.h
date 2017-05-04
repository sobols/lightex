#pragma once

#include <list>
#include <string>

#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#define BOOST_SPIRIT_X3_DEBUG

namespace lightex {
namespace ast {

namespace x3 = boost::spirit::x3;

struct Program;
struct Paragraph;
struct ParagraphBreaker;
struct Argument;
struct ArgumentRef;
struct OuterArgumentRef;
struct InlinedMathText;
struct MathText;
struct CommandMacro;
struct EnvironmentMacro;
struct Command;
struct Environment;

struct ProgramNode : x3::variant<x3::forward_ast<ParagraphBreaker>,
                                 x3::forward_ast<Paragraph>,
                                 x3::forward_ast<MathText>,
                                 x3::forward_ast<Environment>,
                                 x3::forward_ast<CommandMacro>,
                                 x3::forward_ast<EnvironmentMacro>> {
  using base_type::base_type;
  using base_type::operator=;
};

struct ParagraphNode : x3::variant<std::string, x3::forward_ast<InlinedMathText>, x3::forward_ast<Command>> {
  using base_type::base_type;
  using base_type::operator=;
};

struct ArgumentNode : x3::variant<std::string,
                                  x3::forward_ast<InlinedMathText>,
                                  x3::forward_ast<Command>,
                                  x3::forward_ast<ArgumentRef>,
                                  x3::forward_ast<OuterArgumentRef>> {
  using base_type::base_type;
  using base_type::operator=;
};

struct Program : x3::position_tagged {
  std::list<ProgramNode> nodes;
};

struct Paragraph : x3::position_tagged {
  std::list<ParagraphNode> nodes;
};

struct ParagraphBreaker : x3::position_tagged {};

struct Argument : x3::position_tagged {
  std::list<ArgumentNode> nodes;
};

struct ArgumentRef : x3::position_tagged {
  int argument_id;
};

struct OuterArgumentRef : x3::position_tagged {
  int argument_id;
};

struct InlinedMathText : x3::position_tagged {
  std::string text;
};

struct MathText : x3::position_tagged {
  std::string text;
};

struct CommandMacro : x3::position_tagged {
  std::string name;
  boost::optional<int> arguments_num;
  std::list<Argument> default_arguments;
  Argument body;
};

struct EnvironmentMacro : x3::position_tagged {
  std::string name;
  boost::optional<int> arguments_num;
  std::list<Argument> default_arguments;
  Program pre_program;
  Program post_program;
};

struct Command : x3::position_tagged {
  std::string name;
  std::list<Argument> default_arguments;
  std::list<Argument> arguments;
};

struct Environment : x3::position_tagged {
  std::string begin_name;
  std::list<Argument> default_arguments;
  std::list<Argument> arguments;
  Program program;
  std::string end_name;
};

}  // namespace ast
}  // namespace lightex
