#pragma once

#include "string.h"

#include <list>
#include <string>
#include <vector>

#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/context.hpp>

namespace lightex2 {
namespace ast {
namespace x3 = boost::spirit::x3;

struct ExampleTable;
struct Paragraph;
struct FlowContent;
struct PhrasingContent;
struct PlainText;
struct VerbatimText;
struct ParagraphBreaker;
struct Symbol;
struct SingleSymbol;
struct Group;
struct NonBreakingSpace;
struct Comment;
struct InlineMath;
struct DisplayMath;
struct Heading;

struct Bold;
struct Italic;
struct Typewriter;
struct Emphasis;

struct FlowElement : x3::variant<x3::forward_ast<Paragraph>,
                                 x3::forward_ast<ParagraphBreaker>,
                                 x3::forward_ast<ExampleTable>,
                                 x3::forward_ast<Comment>,
                                 x3::forward_ast<Heading>> {
  FlowElement& operator=(const FlowElement&) = default;
  FlowElement(const FlowElement&) = default;
  FlowElement() = default;

  using base_type::base_type;
  using base_type::operator=;
};

struct FlowContent : x3::position_tagged {
  std::list<FlowElement> nodes;
};

struct PhrasingElement : x3::variant<x3::forward_ast<PlainText>,
                                     x3::forward_ast<VerbatimText>,
                                     x3::forward_ast<Bold>,
                                     x3::forward_ast<Italic>,
                                     x3::forward_ast<Typewriter>,
                                     x3::forward_ast<Emphasis>,
                                     x3::forward_ast<Symbol>,
                                     x3::forward_ast<SingleSymbol>,
                                     x3::forward_ast<Group>,
                                     x3::forward_ast<NonBreakingSpace>,
                                     x3::forward_ast<Comment>,
                                     x3::forward_ast<InlineMath>,
                                     x3::forward_ast<DisplayMath>> {
  PhrasingElement& operator=(const PhrasingElement&) = default;
  PhrasingElement(const PhrasingElement&) = default;
  PhrasingElement() = default;

  using base_type::base_type;
  using base_type::operator=;
};

struct PhrasingContent : x3::position_tagged {
  std::list<PhrasingElement> nodes;
};

struct PlainText : x3::position_tagged {
  String text;
};

struct ParagraphBreaker : x3::position_tagged {};

struct VerbatimText : x3::position_tagged {
  String text;
};

struct Paragraph : public PhrasingContent {};

struct Body : x3::position_tagged {
  std::list<FlowElement> nodes;
};

struct Symbol : x3::position_tagged {
  String value;
};

struct SingleSymbol : x3::position_tagged {
  char value;
};

struct Group : public PhrasingContent {
};

struct NonBreakingSpace : x3::position_tagged {
};

struct Example : x3::position_tagged {
  std::list<PhrasingElement> input;
  std::list<PhrasingElement> output;
};

struct ExampleTable : x3::position_tagged {
  std::vector<Example> examples;
  using value_type = Example;
};

struct Comment : x3::position_tagged {

};

// math
struct InlineMath : x3::position_tagged {
  String equation;
};
struct DisplayMath : x3::position_tagged {
  String equation;
};

/**
 * Document structure
 */
struct Section : public PhrasingContent {};
struct Subsection : public PhrasingContent {};

struct PredefinedSection {
  std::string id;
};

struct Heading : x3::variant<x3::forward_ast<Section>,
                             x3::forward_ast<Subsection>,
                             x3::forward_ast<PredefinedSection>> {

  Heading& operator=(const Heading&) = default;
  Heading(const Heading&) = default;
  Heading() = default;

  using base_type::base_type;
  using base_type::operator=;
};


/**
 * Text style
 */
struct Bold : public PhrasingContent {};
struct Italic : public PhrasingContent {};
struct Typewriter : public PhrasingContent {};
struct Emphasis : public PhrasingContent {};
}  // namespace ast
}  // namespace lightex2
