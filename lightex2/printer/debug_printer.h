#pragma once

#include <string>
#include <vector>

#include <lightex2/parser/ast.h>

#include <boost/variant/static_visitor.hpp>

namespace lightex2 {
std::string EscapeC(const std::string& s);

class DebugAstPrinter : public boost::static_visitor<void> {
 public:
  DebugAstPrinter(std::vector<std::string>& out);

  void operator()(const ast::Body&);
  void operator()(const ast::PlainText&);
  void operator()(const ast::VerbatimText&);
  void operator()(const ast::ParagraphBreaker&);
  void operator()(const ast::Paragraph&);

  void VisitPhrasing(const ast::PhrasingContent&);

  void operator()(const ast::Bold&);
  void operator()(const ast::Italic&);
  void operator()(const ast::Typewriter&);
  void operator()(const ast::Emphasis&);

  void operator()(const ast::Symbol&);
  void operator()(const ast::Group&);
  void operator()(const ast::NonBreakingSpace&);

  void operator()(const ast::ExampleTable&);
  void operator()(const ast::Example&);

  void operator()(const ast::Comment&);

 private:
  void Push(const std::string& msg);
  void Indent();
  void Outdent();

 private:
  int indent_;
  std::vector<std::string>& out_;
};

}  // namespace lightex2
