#pragma once

#include <string>

namespace lightex {

  std::u32string ConvertToUTF32(const std::string& s);
  std::string ConvertToUTF8(const std::u32string& s);

}  // namespace lightex
