#include <lightex/pymod/extension.h>

#include <lightex/workspace.h>
#include <lightex/ast/ast.h>

extern const char LIGHTEX_STY_DATA[];
extern const size_t LIGHTEX_STY_LENGTH;

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
      auto ctxt = std::make_unique<Context>();

      // TODO: do once per all contexts
      const std::string lightex_sty{ LIGHTEX_STY_DATA, LIGHTEX_STY_LENGTH };
      ctxt->Execute(lightex_sty);

      return ctxt;
    }
  } // namespace extension
} // namespace lightex
