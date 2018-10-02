#include "common.h"

#include <lightex2/parser/grammar.h>
#include <lightex2/printer/debug_printer.h>

#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/test/unit_test.hpp>

namespace x3 = boost::spirit::x3;
using namespace lightex2;

void CheckOK(const std::string& tex, const std::vector<std::string>& expectedElems) {
  boost::u8_to_u32_iterator<std::string::const_iterator> begin(tex.begin()), end(tex.end());
  ast::Body program;

  bool ok = x3::parse(begin, end, grammar::body, program) && (begin == end);
  if (!ok) {
    std::ostringstream oss;
    oss << '"' << EscapeC(tex) << '"' << ": parse error";
    BOOST_ERROR(oss.str());

  } else {
    std::vector<std::string> foundElems;

    DebugAstPrinter printer{foundElems};
    printer(program);

    bool ok = true;
    std::ostringstream oss;
    oss << '"' << EscapeC(tex) << '"' << '\n';
    for (size_t i = 0; i < std::min(expectedElems.size(), foundElems.size()); ++i) {
      if (expectedElems[i] != foundElems[i]) {
        oss << "AST diff at #" << (i + 1) << '\n';
        oss << "    expected: " << expectedElems[i] << '\n';
        oss << "       found: " << foundElems[i] << '\n';
        ok = false;
        break;
      }
    }
    if (expectedElems.size() != foundElems.size()) {
      oss << "Number of AST nodes: ";
      oss << expectedElems.size() << " expected, ";
      oss << foundElems.size() << " found\n";
      ok = false;
    }

    if (!ok) {
      oss << "Full AST found: {";
      for (size_t j = 0; j < foundElems.size(); ++j) {
        oss << foundElems[j] << ((j + 1 != foundElems.size()) ? ", " : "");
      }
      oss << "}";
      BOOST_ERROR(oss.str());
    }
  }
}

void CheckError(const std::string& tex) {
  boost::u8_to_u32_iterator<std::string::const_iterator> begin(tex.begin()), end(tex.end());
  ast::Body program;

  bool ok = x3::parse(begin, end, grammar::body, program) && (begin == end);
  if (ok) {
    std::ostringstream oss;
    oss << '"' << EscapeC(tex) << '"' << ": expected to fail";
    BOOST_ERROR(oss.str());
  }
}
