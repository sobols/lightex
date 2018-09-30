#include <string>

std::string convertToUtf8(const std::u16string&);
std::u16string convertFromUtf8(const std::string&);

std::u16string escapeHtml(const std::u16string&);
