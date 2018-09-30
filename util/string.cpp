#include "string.h"

std::string EscapeHtml(const std::string& unescaped) {
    std::string buffer;
    buffer.reserve(unescaped.size());

    for (char c : unescaped) {
        switch (c) {
        case '&':
            buffer += "&amp;";
            break;

        case '<':
            buffer += "&lt;";
            break;

        case '>':
            buffer += "&gt;";
            break;

        default:
            buffer += c;
        }
    }
    return buffer;
}
