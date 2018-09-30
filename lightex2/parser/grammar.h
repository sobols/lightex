#pragma once

#include <string>

#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include "ast.h"
#include "ast_adapted.h"

namespace x3 = boost::spirit::x3;

namespace lightex2 {
namespace grammar {
struct symbols_ : x3::symbols<const std::string> {
  symbols_();
};
const symbols_ symbols;

const x3::rule<class FlowElementId, ast::FlowElement> flow_element = "flow_element";
const x3::rule<class PhrasingElementId, ast::PhrasingElement> phrasing_element = "phrasing_element";

const x3::rule<class BodyId, ast::Body> body = "body";
const x3::rule<class ParagraphId, ast::Paragraph> paragraph = "paragraph";
const x3::rule<class PlainTextId, ast::PlainText> plain_text = "plain_text";
const x3::rule<class VerbatimTextId, ast::VerbatimText> verbatim_text = "verbatim_text";
const x3::rule<class ParagraphBreakerId, ast::ParagraphBreaker> paragraph_breaker = "paragraph_breaker";
const x3::rule<class SymbolId, ast::Symbol> symbol = "symbol";

const x3::rule<class GroupId, ast::Group> group = "group";
const x3::rule<class CommentId, ast::Comment> comment = "comment";

// text style
const x3::rule<class BoldId, ast::Bold> bold = "bold";
const x3::rule<class ItalicId, ast::Italic> italic = "italic";
const x3::rule<class TypewriterId, ast::Typewriter> typewriter = "typewriter";
const x3::rule<class EmphasisId, ast::Emphasis> emphasis = "emphasis";

const x3::rule<class NonBreakingSpaceId, ast::NonBreakingSpace> non_breaking_space = "non_breaking_space";

const x3::rule<class ExampleId, ast::Example> example = "example";
const x3::rule<class ExampleTableId, ast::ExampleTable> example_table = "example_table";

/**
 * Terminals
 */

// Characters that are reserved for special purposes
const char SPECIAL[] = {'\\', '{', '}', '$', '&', '#', '%', '^', '_', '~'};
const auto tex_special = x3::char_(SPECIAL);

const auto non_breaking_space_def = x3::omit[x3::lit('~')];

// Comment: ignore everything that follows a % up to the end of line
const auto comment_def = x3::omit[x3::lit('%') >> *(x3::unicode::char_ - x3::eol) >> (x3::eol | x3::eoi)];

const auto space = x3::omit[x3::skip(comment_def)[*x3::unicode::space]];

// TeX ignores the space after control sequence during its tokenization
const auto no_arg_command_end = x3::omit[(&x3::lit('\\') | &tex_special | &x3::eol | &x3::eoi | x3::unicode::space)];

const auto flow_element_def = paragraph | paragraph_breaker | example_table;

const auto phrasing_element_def = plain_text | verbatim_text | bold | italic | typewriter | emphasis | symbol | group | non_breaking_space | comment;

const auto paragraph_def = +phrasing_element;

const auto plain_text_def = +(x3::unicode::char_ - paragraph_breaker - tex_special);

const auto verbatim_text_def = x3::lit("\\begin{verbatim}") >> -x3::eol >>
                               *(x3::unicode::char_ - x3::lit("\\end{verbatim}")) >> x3::lit("\\end{verbatim}");

const auto paragraph_breaker_def = x3::omit[x3::repeat(2, x3::inf)[x3::eol]];

const auto body_def = *flow_element;

const auto group_def = x3::lit('{') >> *phrasing_element >> x3::lit('}');

// Text style
const auto bold_def = x3::lit("\\textbf{") >> *phrasing_element >> x3::lit("}");
const auto italic_def = x3::lit("\\textit{") >> *phrasing_element >> x3::lit("}");
const auto typewriter_def = (x3::lit("\\texttt{") | x3::lit("\\t{")) >> *phrasing_element >> x3::lit("}");
const auto emphasis_def = x3::lit("\\emph{") >> *phrasing_element >> x3::lit("}");

// Symbol
const auto symbol_def = x3::lit('\\') >> symbols >> no_arg_command_end;

/**
 * olymp.sty Examples
 */
const auto example_def = x3::lit("\\exmp") >> '{' >> -x3::eol >> *phrasing_element >> '}' >> '{' >> -x3::eol >> *phrasing_element >> '}';

const auto examples = space >> *(example >> space);

const auto example_table_def = x3::lit("\\begin{example}") >> examples >> x3::lit("\\end{example}");

BOOST_SPIRIT_DEFINE(flow_element);
BOOST_SPIRIT_DEFINE(phrasing_element);
BOOST_SPIRIT_DEFINE(paragraph);
BOOST_SPIRIT_DEFINE(plain_text);
BOOST_SPIRIT_DEFINE(verbatim_text);
BOOST_SPIRIT_DEFINE(paragraph_breaker);
BOOST_SPIRIT_DEFINE(body);

BOOST_SPIRIT_DEFINE(bold);
BOOST_SPIRIT_DEFINE(italic);
BOOST_SPIRIT_DEFINE(typewriter);
BOOST_SPIRIT_DEFINE(emphasis);

BOOST_SPIRIT_DEFINE(symbol);
BOOST_SPIRIT_DEFINE(group);
BOOST_SPIRIT_DEFINE(non_breaking_space);

BOOST_SPIRIT_DEFINE(example);
BOOST_SPIRIT_DEFINE(example_table);
BOOST_SPIRIT_DEFINE(comment);

}  // namespace grammar
}  // namespace lightex2
