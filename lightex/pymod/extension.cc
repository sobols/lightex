#include <lightex/pymod/extension.h>

#include <lightex/workspace.h>
#include <lightex/ast/ast.h>

namespace lightex {
  namespace extension {
    Result Context::Execute(const std::string& texInput) {
      Result result;
      result.ok = false;

      ast::Program ast;
      if (!ParseProgramToAst(texInput, &result.error, &ast)) {
        return result;
      }

      html_converter::HtmlVisitor visitor_copy = visitor_;
      html_converter::Result html_result = visitor_copy(ast);
      if (!html_result.is_successful) {
        result.error = html_result.error_message;
        return result;
      }

      visitor_ = std::move(visitor_copy);

      result.html = html_result.escaped;
      result.ok = true;
      return result;
    }
    
    std::unique_ptr<Context> Module::CreateContext(bool) const {
      return std::make_unique<Context>();
    }
  } // namespace extension
} // namespace lightex
