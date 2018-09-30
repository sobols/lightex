#include "renderer.h"

#include <util/string.h>

#include <sstream>
#include <iomanip>

namespace {
    std::u16string escapeStringForJs(const std::u16string& unescaped) {
        std::u16string buffer;

        for (char16_t c : unescaped) {
            if (c == '"' || c == '\\' || ('\x00' <= c && c <= '\x1f')) {
                std::ostringstream oss;
                oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << uint32_t{c};
                buffer += convertFromUtf8(oss.str());
            } else {
                buffer += c;
            }
        }
        return buffer;
    }

    class KaTeXInBrowserRenderer: public IMathRenderer {
    public:
        KaTeXInBrowserRenderer()
            : counter_(0)
        {
        }

        IMathRenderer::Result render(const std::u16string& expression, Mode mode) override {
            std::basic_stringstream<char16_t> out;
            ++counter_;
            std::u16string id = convertFromUtf8(std::to_string(counter_));
            out << u"<span id=\"math_" << id << u"\"></span>";
            out << u"<script type=\"text/javascript\">";
            out << u"katex.render(";
            out << u"\"" << escapeStringForJs(expression) << u"\", ";
            out << u"document.getElementById(\"math_" << id << u"\"), ";
            out << u"{displayMode: " << (mode == Mode::Display ? u"true" : u"false") << u", throwOnError: false}";
            out << u")";
            out << u"</script>";

            IMathRenderer::Result result;
            result.ok = true;
            result.html = out.str();
            return result;
        }

    private:
        uint64_t counter_;
    };
}

std::unique_ptr<IMathRenderer> createKaTeXInBrowserRenderer() {
    std::unique_ptr<IMathRenderer> ptr;
    ptr.reset(new KaTeXInBrowserRenderer());
    return ptr;
}
