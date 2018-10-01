#include "grammar.h"
#include <iostream>

namespace lightex2 {
namespace grammar {

symbols_::symbols_() {
  add("alpha", "&alpha;")("beta", "&beta;");
}

}  // namespace grammar
}  // namespace lightex2
