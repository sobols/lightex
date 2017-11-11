#include <lightex/workspace.h>

#include <map>
#include <mutex>

#include <lightex/ast/ast.h>
#include <lightex/dot_converter/dot_visitor.h>
#include <lightex/html_converter/html_visitor.h>
#include <lightex/grammar/grammar.h>
#include <lightex/utils/file_utils.h>
#include <lightex/utils/utf32.h>

#include <boost/spirit/home/x3.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace lightex {
namespace {

const char kSyntaxParsingError[] = "Error while running syntax analysis! Failed on the following snippet: ";
const int kFailedSnippetLength = 30;

namespace x3 = boost::spirit::x3;

bool ParseProgramToAst(const std::string& input, std::string* error_message, ast::Program* output) {
  if (!output) {
    return false;
  }

  const boost::u8_to_u32_iterator<std::string::const_iterator> begin(input.begin()), end(input.end());
  auto iter = begin;

  if (!x3::phrase_parse(iter, end, grammar::program, x3::unicode::space, *output) || iter != end) {
    if (error_message) {
      std::size_t failed_at = 0; // iter - start;
      *error_message = kSyntaxParsingError;
      *error_message = input.substr(failed_at, failed_at + kFailedSnippetLength);
      if (failed_at + kFailedSnippetLength + 1 < input.size()) {
        *error_message += "...";
      }
    }
    return false;
  }

  return true;
}

class DotWorkspace : public Workspace {
 public:
  ~DotWorkspace() {}

  bool LoadStyle(const std::string& style_file_data, std::string* error_message) override {
    return false;
  }

  bool ParseProgram(const std::string& input, std::string* error_message, std::string* output) override {
    ast::Program ast;
    if (!ParseProgramToAst(input, error_message, &ast)) {
      return false;
    }

    *output += "digraph d {\n";
    dot_converter::DotVisitor visitor(output);
    visitor(ast);
    *output += "}\n";

    return true;
  }
};

class HtmlWorkspace : public Workspace {
 public:
  ~HtmlWorkspace() {}

  bool LoadStyle(const std::string& style_file_data, std::string* error_message) override {
    ast::Program ast;
    if (!ParseProgramToAst(style_file_data, error_message, &ast)) {
      return false;
    }

    std::unique_lock<std::mutex> lock(mtx_);
    html_converter::HtmlVisitor visitor_copy = visitor_;
    html_converter::Result result = visitor_copy(ast);
    if (!result.is_successful) {
      if (error_message) {
        *error_message = result.error_message;
      }
      return false;
    }
    visitor_ = visitor_copy;

    return true;
  }

  bool ParseProgram(const std::string& input, std::string* error_message, std::string* output) override {
    if (!output) {
      return false;
    }

    ast::Program ast;
    if (!ParseProgramToAst(input, error_message, &ast)) {
      return false;
    }

    html_converter::HtmlVisitor visitor_copy = visitor_;
    html_converter::Result result = visitor_copy(ast);
    if (!result.is_successful) {
      if (error_message) {
        *error_message = result.error_message;
      }
      return false;
    }

    if (output) {
      *output = ConvertToUTF8(result.escaped);
    }
    return true;
  }

 private:
  html_converter::HtmlVisitor visitor_;
  std::mutex mtx_;
};
}  // namespace

std::shared_ptr<Workspace> MakeDotWorkspace() {
  return std::make_shared<DotWorkspace>();
}

std::shared_ptr<Workspace> MakeHtmlWorkspace() {
  return std::make_shared<HtmlWorkspace>();
}
}  // namespace lightex
