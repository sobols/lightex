#include "debug_printer.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace lightex2 {
DebugAstPrinter::DebugAstPrinter(std::vector<std::string>& out)
    : indent_(0), out_(out) {}

std::string EscapeC(const std::string& s) {
  std::ostringstream result;
  for (char c : s) {
    const unsigned char uc = c;
    switch (uc) {
      case '\\':
        result << "\\\\";
        break;
      case '\n':
        result << "\\n";
        break;
      case '\r':
        result << "\\r";
        break;
      case '\t':
        result << "\\t";
        break;
      default:
        if (uc < 32 || uc == 127) {
          result << "\\x" << std::hex << std::setw(2) << std::setfill('0') << unsigned(uc);
        } else {
          result << c;
        }
    }
  }
  return result.str();
}

void DebugAstPrinter::operator()(const ast::Body& content) {
  for (const auto& node : content.nodes) {
    boost::apply_visitor(*this, node);
  }
}

void DebugAstPrinter::operator()(const ast::Paragraph& paragraph) {
  Push("Paragraph");
  Indent();
  for (const auto& node : paragraph.nodes) {
    boost::apply_visitor(*this, node);
  }
  Outdent();
}

void DebugAstPrinter::operator()(const ast::PlainText& node) {
  Push("PlainText '" + EscapeC(node.text.Get()) + "'");
}

void DebugAstPrinter::operator()(const ast::VerbatimText& node) {
  Push("VerbatimText '" + EscapeC(node.text.Get()) + "'");
}

void DebugAstPrinter::operator()(const ast::ParagraphBreaker&) {
  Push("ParagraphBreaker");
}

void DebugAstPrinter::VisitPhrasing(const ast::PhrasingContent& content) {
  Indent();
  for (const auto& node : content.nodes) {
    boost::apply_visitor(*this, node);
  }
  Outdent();
}

void DebugAstPrinter::operator()(const ast::Bold& content) {
  Push("Bold");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::Italic& content) {
  Push("Italic");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::Typewriter& content) {
  Push("Typewriter");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::Emphasis& content) {
  Push("Emphasis");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::Symbol& symbol) {
  Push("Symbol " + symbol.value.Get());
}

void DebugAstPrinter::operator()(const ast::SingleSymbol& symbol) {
  Push(std::string{"SingleSymbol "} + symbol.value);
}

void DebugAstPrinter::operator()(const ast::Group& content) {
  Push("Group");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::NonBreakingSpace&) {
  Push("NonBreakingSpace");
}

void DebugAstPrinter::operator()(const ast::ExampleTable& table) {
  Push("ExampleTable");
  Indent();
  for (const auto& example : table.examples) {
    (*this)(example);
  }
  Outdent();
}

void DebugAstPrinter::operator()(const ast::Example& example) {
  Push("Example Input");
  Indent();
  for (const auto& node : example.input) {
    boost::apply_visitor(*this, node);
  }
  Outdent();

  Push("Example Output");
  Indent();
  for (const auto& node : example.output) {
    boost::apply_visitor(*this, node);
  }
  Outdent();
}

void DebugAstPrinter::operator()(const ast::Comment&) {

}

void DebugAstPrinter::operator()(const ast::InlineMath& node) {
  Push("InlineMath '" + EscapeC(node.equation.Get()) + "'");
}

void DebugAstPrinter::operator()(const ast::DisplayMath& node) {
  Push("DisplayMath '" + EscapeC(node.equation.Get()) + "'");
}

void DebugAstPrinter::operator()(const ast::Heading& content) {
  boost::apply_visitor(*this, content);
}

void DebugAstPrinter::operator()(const ast::Section& content) {
  Push("Section");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::Subsection& content) {
  Push("Subsection");
  VisitPhrasing(content);
}

void DebugAstPrinter::operator()(const ast::PredefinedSection& content) {
  Push("PredefinedSection " + content.id);
}

void DebugAstPrinter::Push(const std::string& msg) {
  std::string res;
  for (int i = 0; i < indent_; ++i) {
    res += "- ";
  }
  res += msg;
  out_.push_back(res);
}
void DebugAstPrinter::Indent() {
  indent_++;
}
void DebugAstPrinter::Outdent() {
  indent_--;
}
}  // namespace lightex2
