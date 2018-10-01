#define BOOST_TEST_MODULE master

#include "common.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestSingleParagraph) {
  CheckOK("", {});

  CheckOK(" ",
          {"Paragraph",
           "- PlainText ' '"});

  CheckOK("a",
          {"Paragraph",
           "- PlainText 'a'"});

  CheckOK("a b",
          {"Paragraph",
           "- PlainText 'a b'"});

  CheckOK(" a  b ",
          {"Paragraph",
           "- PlainText ' a  b '"});

  CheckOK("Привет, мир!",
          {"Paragraph",
           "- PlainText 'Привет, мир!'"});
}

BOOST_AUTO_TEST_CASE(TestControlChars) {
  CheckOK("\t",
          {"Paragraph",
           "- PlainText '\\t'"});

  CheckOK(std::string(3, '\0'),
          {"Paragraph",
           "- PlainText '\\x00\\x00\\x00'"});
}

BOOST_AUTO_TEST_CASE(TestMultipleParagraphs) {
  CheckOK("a\nb",
          {"Paragraph",
           "- PlainText 'a\\nb'"});

  // Win
  CheckOK("a\r\nb",
          {"Paragraph",
           "- PlainText 'a\\r\\nb'"});

  CheckOK("a\n\nb",
          {"Paragraph",
           "- PlainText 'a'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'b'"});

  // Win
  CheckOK("a\r\n\r\nb",
          {"Paragraph",
           "- PlainText 'a'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'b'"});

  // Mixed
  CheckOK("a\r\n\nb",
          {"Paragraph",
           "- PlainText 'a'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'b'"});
  CheckOK("a\n\r\nb",
          {"Paragraph",
           "- PlainText 'a'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'b'"});

  // Multiple newlines
  CheckOK("a\n\n\nb",
          {"Paragraph",
           "- PlainText 'a'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'b'"});
}

BOOST_AUTO_TEST_CASE(TestVerbatim) {
  CheckOK("\\begin{verbatim}\\end{verbatim}", {"Paragraph", "- VerbatimText ''"});

  CheckOK("Path \\begin{verbatim}C:\\temp\\end{verbatim} does not exist.",
          {"Paragraph",
           "- PlainText 'Path '",
           "- VerbatimText 'C:\\\\temp'",
           "- PlainText ' does not exist.'"});

  CheckOK(
      "\\begin{verbatim}\n"
      "\\textit{italic}\n"
      "\\end{verbati}\n"
      " \\end{verbatim}",
      {"Paragraph",
       "- VerbatimText '\\\\textit{italic}\\n\\\\end{verbati}\\n '"});

  CheckOK("A\\begin{verbatim}B\\end{verbatim}C",
          {"Paragraph",
           "- PlainText 'A'",
           "- VerbatimText 'B'",
           "- PlainText 'C'"});

  CheckOK("A\n\\begin{verbatim}B\\end{verbatim}\nC",
          {"Paragraph",
           "- PlainText 'A\\n'",
           "- VerbatimText 'B'",
           "- PlainText '\\nC'"});

  CheckOK("A\\begin{verbatim}B\\end{verbatim}\n\nC",
          {"Paragraph",
           "- PlainText 'A'",
           "- VerbatimText 'B'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'C'"});

  CheckOK("A\n\n\\begin{verbatim}B\\end{verbatim}\n\nC",
          {"Paragraph",
           "- PlainText 'A'",
           "ParagraphBreaker",
           "Paragraph",
           "- VerbatimText 'B'",
           "ParagraphBreaker",
           "Paragraph",
           "- PlainText 'C'"});
}

BOOST_AUTO_TEST_CASE(TestTextStyle) {
  CheckOK("a\\textit{b}c",
          {"Paragraph",
           "- PlainText 'a'",
           "- Italic",
           "- - PlainText 'b'",
           "- PlainText 'c'"});

  CheckOK(
      "\\textbf{bold \\textit{italic} and non-italic "
      "\\t{monospaced}}\n\n\\emph{Hello}",
      {"Paragraph",
       "- Bold",
       "- - PlainText 'bold '",
       "- - Italic",
       "- - - PlainText 'italic'",
       "- - PlainText ' and non-italic '",
       "- - Typewriter",
       "- - - PlainText 'monospaced'",
       "ParagraphBreaker",
       "Paragraph",
       "- Emphasis",
       "- - PlainText 'Hello'"});
}

BOOST_AUTO_TEST_CASE(TestSymbols) {
  CheckOK("\\alpha\\textit{\\beta}",
          {"Paragraph",
           "- Symbol &alpha;",
           "- Italic",
           "- - Symbol &beta;"});

  CheckOK("\\alpha\\alpha",
          {"Paragraph",
           "- Symbol &alpha;",
           "- Symbol &alpha;"});

  CheckOK("A\\alpha B\\alpha \\alpha{} \\alpha",
          {"Paragraph",
           "- PlainText 'A'",
           "- Symbol &alpha;",
           "- PlainText 'B'",
           "- Symbol &alpha;",
           "- Symbol &alpha;",
           "- Group",
           "- PlainText ' '",
           "- Symbol &alpha;"});

  CheckOK("\\alpha\n\n\\alpha",
          {"Paragraph",
           "- Symbol &alpha;",
           "ParagraphBreaker",
           "Paragraph",
           "- Symbol &alpha;"});

  CheckOK("\\alpha~\\alpha",
          {"Paragraph",
           "- Symbol &alpha;",
           "- NonBreakingSpace",
           "- Symbol &alpha;"});
}

BOOST_AUTO_TEST_CASE(TestSingleSymbols) {
  CheckOK("\\$\\$",
          {"Paragraph",
           "- SingleSymbol $",
           "- SingleSymbol $"});

  CheckOK("A\\$ B\\$ \\${} \\$",
          {"Paragraph",
           "- PlainText 'A'",
           "- SingleSymbol $",
           "- PlainText ' B'",
           "- SingleSymbol $",
           "- PlainText ' '",
           "- SingleSymbol $",
           "- Group",
           "- PlainText ' '",
           "- SingleSymbol $"});

  CheckOK("\\$\n\n\\$",
          {"Paragraph",
           "- SingleSymbol $",
           "ParagraphBreaker",
           "Paragraph",
           "- SingleSymbol $"});

  CheckOK("\\$~\\$",
          {"Paragraph",
           "- SingleSymbol $",
           "- NonBreakingSpace",
           "- SingleSymbol $"});
}
