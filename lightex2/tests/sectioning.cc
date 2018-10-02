#include "common.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestSection) {
  CheckOK("\\section{}",
          {"Section"});

  CheckOK("\\section{Hello}",
          {"Section",
           "- PlainText 'Hello'"});

  CheckError("\\section");

  CheckOK("\\section{Hello}World",
          {"Section",
           "- PlainText 'Hello'",
           "Paragraph",
           "- PlainText 'World'"});
}

BOOST_AUTO_TEST_CASE(TestSectionSubsection) {
  CheckOK("\\section{Aba}\\subsection{Caba}",
          {"Section",
           "- PlainText 'Aba'",
           "Subsection",
           "- PlainText 'Caba'"});

  CheckOK("\\section{Aba}%\n\\subsection{Caba}",
          {"Section",
           "- PlainText 'Aba'",
           "Subsection",
           "- PlainText 'Caba'"});

  CheckOK("\\section{Aba}\n\\subsection{Caba}",
          {"Section",
           "- PlainText 'Aba'",
           "Paragraph",
           "- PlainText '\\n'",
           "Subsection",
           "- PlainText 'Caba'"});

  CheckOK("\\section{Aba}\n\n\\subsection{Caba}",
          {"Section",
           "- PlainText 'Aba'",
           "ParagraphBreaker",
           "Subsection",
           "- PlainText 'Caba'"});
}

BOOST_AUTO_TEST_CASE(PredefinedSections) {
  CheckOK("\\InputFile",
          {"PredefinedSection InputFile"});
  CheckOK("\\\x49nputFile",
          {"PredefinedSection InputFile"});

  // TODO: this test must fail: overflow when casting to char
  CheckOK("\\\xC5\x89nputFile",
          {"PredefinedSection InputFile"});
  CheckOK("\\\xF0\x90\x81\x89nputFile",
          {"PredefinedSection InputFile"});
}
