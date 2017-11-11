#include <lightex/utils/utf32.h>

#include <boost/regex/pending/unicode_iterator.hpp>

namespace lightex {
  std::u32string ConvertToUTF32(const std::string& s) {
    using Conv = boost::u8_to_u32_iterator<std::string::const_iterator>;
    return std::u32string{ Conv(s.begin()), Conv(s.end()) };
  }

  std::string ConvertToUTF8(const std::u32string& s) {
    using Conv = boost::u32_to_u8_iterator<std::u32string::const_iterator>;
    return std::string{ Conv(s.begin()), Conv(s.end()) };
  }
}  // namespace lightex
