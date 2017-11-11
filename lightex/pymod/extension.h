#pragma once

#include <lightex/html_converter/html_visitor.h>

#include <string>
#include <memory>

namespace lightex {
  namespace extension {
    struct Result {
      bool ok = false;
      std::string html;
      std::string error;
    };

    class Context {
    public:
      Result Execute(const std::string& texInput);

    private:
      html_converter::HtmlVisitor visitor_;
    };

    class Module {
    public:
      std::unique_ptr<Context> CreateContext(bool) const;
    };
  } // namespace extension
} // namespace lightex
