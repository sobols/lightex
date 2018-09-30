#include "string.h"

#include <locale>
#include <codecvt>

std::string convertToUtf8(const std::u16string& source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.to_bytes(source);
}

std::u16string convertFromUtf8(const std::string& source) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    return convert.from_bytes(source);
}

std::u16string escapeHtml(const std::u16string& unescaped) {
    std::u16string buffer;
    for (auto c : unescaped) {
        switch (c) {
        case '&':
            buffer += u"&amp;";
            break;

        case '\"':
            buffer += u"&quot;";
            break;

        case '\'':
            buffer += u"&apos;";
            break;

        case '<':
            buffer += u"&lt;";
            break;

        case '>':
            buffer += u"&gt;";
            break;

        default:
            buffer += c;
        }
    }
    return buffer;
}
