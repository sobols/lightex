#include "common.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestDisplayMath) {
  CheckOK("$$2+2$$",
          {"Paragraph",
           "- DisplayMath '2+2'"});

  CheckOK("$$2+2\\$\\sqrt{2}$$",
          {"Paragraph",
           "- DisplayMath '2+2$\\\\sqrt{2}'"});

  CheckOK("\\$a$$a\\$b$$c\\$",
          {"Paragraph",
           "- SingleSymbol $",
           "- PlainText 'a'",
           "- DisplayMath 'a$b'",
           "- PlainText 'c'",
           "- SingleSymbol $"});

  CheckOK("It is obvious that\n"
          "$$2 + 2 = 4.$$\n"
          "\n"
          "Note that\n"
          "$$\n"
          "4 - 2 = 2\n"
          "$$\n"
          "by definition.",
          {"Paragraph",
           "- PlainText 'It is obvious that\\n'",
           "- DisplayMath '2 + 2 = 4.'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'Note that\\n'",
           "- DisplayMath '\\n4 - 2 = 2\\n'",
           "- PlainText '\\nby definition.'"});
}

BOOST_AUTO_TEST_CASE(TestInlineMath) {
  CheckOK("$2+2$",
          {"Paragraph",
           "- InlineMath '2+2'"});
}

BOOST_AUTO_TEST_CASE(TestTwoMaths) {
  CheckOK("$inl$$$disp$$",
          {"Paragraph",
           "- InlineMath 'inl'",
           "- DisplayMath 'disp'"});
  CheckOK("$$disp$$$inl$",
          {"Paragraph",
           "- DisplayMath 'disp'",
           "- InlineMath 'inl'"});
}
