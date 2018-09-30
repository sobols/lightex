#include "common.h"

#include <boost/test/unit_test.hpp>

const char EXAMPLES[] = R"*(\begin{example}
\exmp{%
2
1 2
}{%
1
}%
\exmp{
 aba  caba
}{
.%
}
\end{example})*";

BOOST_AUTO_TEST_CASE(TestExamples) {
  CheckOK("\\begin{example}\\end{example}",
          {"ExampleTable"});

  CheckOK("\\begin{example}\n\n   \n  % comment \n  \\end{example}",
          {"ExampleTable"});

  CheckOK(EXAMPLES,
          {"ExampleTable",
           "- Example Input",
           "- - PlainText '2\\n1 2\\n'",
           "- Example Output",
           "- - PlainText '1\\n'",
           "- Example Input",
           "- - PlainText ' aba  caba\\n'",
           "- Example Output",
           "- - PlainText '.'"});
}
