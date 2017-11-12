#include <lightex/html_converter/html_visitor.h>

#include <lightex/utils/utf32.h>

#include <boost/spirit/home/support/char_encoding/unicode.hpp>

#include <cctype>
#include <list>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

namespace lightex {
namespace html_converter {
namespace {

const std::map<std::u32string, std::u32string> kLookupTableSymbols = {
    {U"\\,", U"&thinsp;"}, {U"~", U"&nbsp;"}, {U"---", U"&mdash;"}, {U"--", U"&ndash;"}, {U"<<", U"&laquo;"}, {U">>", U"&raquo;"}};

std::u32string FormatText(const std::u32string& unformatted) {
  std::u32string buffer;

  bool previous_is_space = false;
  for (char32_t c : unformatted) {
    if (boost::spirit::char_encoding::unicode::isspace(c)) {
      if (!previous_is_space) {
        buffer += ' ';
      }
      previous_is_space = true;
      continue;
    }

    buffer += c;
    previous_is_space = false;
  }

  return buffer;
}

std::u32string EscapeStringForHtml(const std::u32string& unescaped) {
  std::u32string buffer;
  for (char32_t c : unescaped) {
    switch (c) {
      case '&':
        buffer += U"&amp;";
        break;

      case '\"':
        buffer += U"&quot;";
        break;

      case '\'':
        buffer += U"&apos;";
        break;

      case '<':
        buffer += U"&lt;";
        break;

      case '>':
        buffer += U"&gt;";
        break;

      default:
        buffer += c;
    }
  }

  return buffer;
}

std::u32string EscapeStringForJs(const std::u32string& unescaped) {
  std::u32string buffer;

  //for (char32_t c : unescaped) {
  //  if (c == '"' || c == '\\' || ('\x00' <= c && c <= '\x1f')) {
  //    buffer << "\\u" << std::hex << std::setw(4) << std::setfill('0') << int{c};
  //  } else {
  //    buffer << c;
  //  }
  //}
  buffer = unescaped;
  return buffer;
}

std::u32string EscapeDollarSigns(const std::u32string& unescaped) {
  std::u32string buffer;

  for (char32_t c : unescaped) {
    switch (c) {
      case '$':
        buffer += U"\\$";
        break;

      default:
        buffer += c;
    }
  }

  return buffer;
}

std::u32string RenderMathFormula(const std::u32string& math_text, bool is_inlined, int* math_text_span_num) {
  std::u32string span_id = U"mathTextSpan" + ConvertToUTF32(std::to_string(++(*math_text_span_num)));
  std::u32string display_mode = is_inlined ? U"false" : U"true";

  std::u32string buffer;
  buffer += U"<span id=\"";
  buffer += span_id;
  buffer += U"\"></span>";

  buffer += U"<script type=\"text/javascript\">";
  buffer += U"katex.render(";
  buffer += U"\"";
  buffer += EscapeStringForJs(EscapeDollarSigns(math_text));
  buffer += U"\", ";
  buffer += U"document.getElementById(\"";
  buffer += span_id;
  buffer += U"\"), {displayMode: ";
  buffer += display_mode;
  buffer += U"});";
  buffer += U"</script>";

  return buffer;
}

template <typename MacroDefinition>
const MacroDefinition* GetMacroDefinition(const std::list<MacroDefinition>& macro_definitions,
                                          const std::u32string& name) {
  for (auto it = macro_definitions.rbegin(); it != macro_definitions.rend(); ++it) {
    if (it->name == name) {
      return &(*it);
    }
  }

  return nullptr;
}
}  // namespace

Result Result::Failure(const std::string& error_message) {
  return {false, U"", U"", error_message, false};
}

Result Result::Success(const std::u32string& escaped, const std::u32string& unescaped, bool breaks_paragraph) {
  return {true, escaped, unescaped, "", breaks_paragraph};
}

Result HtmlVisitor::operator()(const ast::Program& program) {
  return JoinNodeResults(program.nodes);
}

Result HtmlVisitor::operator()(const ast::PlainText& plain_text) {
  const auto it = kLookupTableSymbols.find(plain_text.text);
  if (it != kLookupTableSymbols.end()) {
    return Result::Success(it->second, it->first);
  }

  return Result::Success(EscapeStringForHtml(FormatText(plain_text.text)), plain_text.text);
}

Result HtmlVisitor::operator()(const ast::Paragraph& paragraph) {
  Result result = JoinNodeResults(paragraph.nodes);
  if (!result.is_successful) {
    return result;
  }

  std::u32string formatted_result = FormatText(result.escaped);
  if (formatted_result.empty() || formatted_result == U" ") {
    return result;
  }

  if (active_environment_definitions_num_ == 0 && !result.breaks_paragraph) {
    result.escaped = U"<p>" + result.escaped + U"</p>";
    result.unescaped = U"<p>" + result.unescaped + U"</p>";
  }

  return result;
}

Result HtmlVisitor::operator()(const ast::ParagraphBreaker& paragraph_breaker) {
  return Result::Success(U"", U"");
}

Result HtmlVisitor::operator()(const ast::Argument& argument) {
  return JoinNodeResults(argument.nodes);
}

Result HtmlVisitor::operator()(const ast::ArgumentRef& argument_ref) {
  const Result* argument = GetArgumentByReference(argument_ref.argument_id - 1, false);
  if (!argument) {
    return Result::Failure("Invalid argument reference.");
  }

  return *argument;
}

Result HtmlVisitor::operator()(const ast::OuterArgumentRef& outer_argument_ref) {
  const Result* argument = GetArgumentByReference(outer_argument_ref.argument_id - 1, true);
  if (!argument) {
    return Result::Failure("Invalid outer argument reference.");
  }

  return *argument;
}

Result HtmlVisitor::operator()(const ast::InlinedMathText& math_text) {
  std::u32string render_result = RenderMathFormula(math_text.text, true, &math_text_span_num_);
  return Result::Success(render_result, render_result);
}

Result HtmlVisitor::operator()(const ast::MathText& math_text) {
  std::u32string render_result = RenderMathFormula(math_text.text, false, &math_text_span_num_);
  return Result::Success(render_result, render_result);
}

Result HtmlVisitor::operator()(const ast::CommandMacro& command_macro) {
  if (command_macro.arguments_num.get_value_or(0) < command_macro.default_arguments.size()) {
    return Result::Failure("Invalid number of arguments for command macro " + ConvertToUTF8(command_macro.name) + ".");
  }

  defined_command_macros_.push_back(command_macro);
  return Result::Success(U"", U"");
}

Result HtmlVisitor::operator()(const ast::EnvironmentMacro& environment_macro) {
  if (environment_macro.arguments_num.get_value_or(0) < environment_macro.default_arguments.size()) {
    return Result::Failure("Invalid number of arguments for environment macro " + ConvertToUTF8(environment_macro.name) + ".");
  }

  defined_environment_macros_.push_back(environment_macro);
  return Result::Success(U"", U"");
}

Result HtmlVisitor::operator()(const ast::Command& command) {
  const ast::CommandMacro* command_macro_ptr = GetDefinedCommandMacro(command.name);
  if (!command_macro_ptr) {
    return Result::Failure("Command macro " + ConvertToUTF8(command.name) + " is not defined yet.");
  }

  std::vector<Result> args;
  Result intermediate_result = PrepareMacroArguments(command, *command_macro_ptr, &args);
  if (!intermediate_result.is_successful) {
    return intermediate_result;
  }

  arguments_stack_.push_back(std::move(args));
  intermediate_result = (*this)(command_macro_ptr->body);
  arguments_stack_.pop_back();

  return intermediate_result;
}

Result HtmlVisitor::operator()(const ast::UnescapedCommand& unescaped_command) {
  Result result = (*this)(unescaped_command.body);

  if (result.is_successful) {
    result.escaped = result.unescaped;
  }

  return result;
}

Result HtmlVisitor::operator()(const ast::NparagraphCommand& nparagraph_command) {
  Result result = (*this)(nparagraph_command.body);

  if (result.is_successful) {
    result.breaks_paragraph = true;
  }

  return result;
}

Result HtmlVisitor::operator()(const ast::Environment& environment) {
  if (environment.name != environment.end_name) {
    return Result::Failure("Environment name doesn't match the end name: " + ConvertToUTF8(environment.name) + " != " +
                           ConvertToUTF8(environment.end_name));
  }

  const ast::EnvironmentMacro* environment_macro_ptr = GetDefinedEnvironmentMacro(environment.name);
  if (!environment_macro_ptr) {
    return Result::Failure("Environment macro " + ConvertToUTF8(environment.name) + " is not defined yet.");
  }

  std::vector<Result> args;
  Result intermediate_result = PrepareMacroArguments(environment, *environment_macro_ptr, &args);
  if (!intermediate_result.is_successful) {
    return intermediate_result;
  }

  std::size_t cached_defined_command_macros_num = defined_command_macros_.size();

  const auto rollback = [&]() {
    while (cached_defined_command_macros_num < defined_command_macros_.size()) {
      defined_command_macros_.pop_back();
    }

    arguments_stack_.pop_back();
  };

  arguments_stack_.push_back(std::move(args));

  std::u32string escaped;
  std::u32string unescaped;
  bool breaks_paragraph;

  active_environment_definitions_num_ += 1;
  intermediate_result = (*this)(environment_macro_ptr->pre_program);
  active_environment_definitions_num_ -= 1;
  if (intermediate_result.is_successful) {
    escaped += intermediate_result.escaped;
    unescaped += intermediate_result.unescaped;
  } else {
    rollback();
    return intermediate_result;
  }

  intermediate_result = (*this)(environment.program);
  if (intermediate_result.is_successful) {
    escaped += intermediate_result.escaped;
    unescaped += intermediate_result.unescaped;
    breaks_paragraph = intermediate_result.breaks_paragraph;
  } else {
    rollback();
    return intermediate_result;
  }

  active_environment_definitions_num_ += 1;
  intermediate_result = (*this)(environment_macro_ptr->post_program);
  active_environment_definitions_num_ -= 1;
  if (intermediate_result.is_successful) {
    escaped += intermediate_result.escaped;
    unescaped += intermediate_result.unescaped;
  } else {
    rollback();
    return intermediate_result;
  }

  rollback();
  return Result::Success(escaped, unescaped, breaks_paragraph);
}

Result HtmlVisitor::operator()(const ast::VerbatimEnvironment& verbatim_environment) {
  std::u32string html_text = U"<pre>" + EscapeStringForHtml(verbatim_environment.content) + U"</pre>";

  return Result::Success(html_text, verbatim_environment.content);
}

template <typename Node>
Result HtmlVisitor::JoinNodeResults(const std::list<Node>& nodes) {
  std::u32string escaped;
  std::u32string unescaped;

  bool breaks_paragraph = false;
  for (const auto& node : nodes) {
    Result child_result = boost::apply_visitor(*this, node);
    if (!child_result.is_successful) {
      return child_result;
    }

    escaped += child_result.escaped;
    unescaped += child_result.unescaped;
    breaks_paragraph |= child_result.breaks_paragraph;
  }

  return Result::Success(escaped, unescaped, breaks_paragraph);
};

template <typename Macro, typename MacroDefinition>
Result HtmlVisitor::PrepareMacroArguments(const Macro& macro,
                                          const MacroDefinition& macro_definition,
                                          std::vector<Result>* output_args) {
  if (!output_args) {
    return Result::Failure("No place for preparing macro arguments is provided.");
  }

  int default_args_num = macro_definition.default_arguments.size();
  int redefined_default_args_num = macro.default_arguments.size();
  if (redefined_default_args_num > default_args_num) {
    return Result::Failure("Macro " + ConvertToUTF8(macro.name) + " has more default arguments than it's been defined. " +
                           "Expected " + std::to_string(default_args_num) + ", got " +
                           std::to_string(redefined_default_args_num) + ".");
  }

  int args_num = macro_definition.default_arguments.size() + macro.arguments.size();
  int expected_args_num = macro_definition.arguments_num.get_value_or(0);
  if (args_num != expected_args_num) {
    return Result::Failure("Macro " + ConvertToUTF8(macro.name) + " has invalid number of arguments. " + "Expected " +
                           std::to_string(expected_args_num) + ", got " + std::to_string(args_num) + ".");
  }

  const auto get_argument = [&](int i) {
    const auto list_at = [](const std::list<ast::Argument>& arguments, int index) {
      return *std::next(arguments.begin(), index);
    };

    if (i < redefined_default_args_num) {
      return list_at(macro.default_arguments, i);
    } else if (i < default_args_num) {
      return list_at(macro_definition.default_arguments, i);
    } else {
      return list_at(macro.arguments, i - default_args_num);
    }
  };

  output_args->resize(args_num);
  for (int i = 0; i < args_num; ++i) {
    Result child_result = (*this)(get_argument(i));
    if (!child_result.is_successful) {
      return child_result;
    }

    output_args->at(i) = std::move(child_result);
  }

  return Result::Success(U"", U"");
}

const ast::CommandMacro* HtmlVisitor::GetDefinedCommandMacro(const std::u32string& name) const {
  return GetMacroDefinition(defined_command_macros_, name);
}

const ast::EnvironmentMacro* HtmlVisitor::GetDefinedEnvironmentMacro(const std::u32string& name) const {
  return GetMacroDefinition(defined_environment_macros_, name);
}

const Result* HtmlVisitor::GetArgumentByReference(int index, bool is_outer) const {
  int arguments_stack_index = static_cast<int>(arguments_stack_.size()) - (is_outer ? 2 : 1);
  if (index < 0 || arguments_stack_index < 0) {
    return nullptr;
  }
  const auto& arguments = arguments_stack_[arguments_stack_index];

  if (arguments.size() <= index) {
    return nullptr;
  }

  return &arguments[index];
}
}  // namespace html_converter
}  // namespace lightex
