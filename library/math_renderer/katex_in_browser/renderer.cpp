#include "renderer.h"

#include <util/string.h>

#include <sstream>
#include <iomanip>

namespace {
    std::string EscapeStringForJs(const std::string& unescaped) {
        std::stringstream buffer;

        for (char c : unescaped) {
            if (c == '"' || c == '\\' || ('\x00' <= c && c <= '\x1f')) {
                buffer << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<unsigned int>(c);
            } else {
                buffer << c;
            }
        }
        return buffer.str();
    }

    class KaTeXInBrowserRenderer: public IMathRenderer {
    public:
        KaTeXInBrowserRenderer()
            : counter_(0)
        {
        }

        IMathRenderer::Result Render(const std::string& expression, Mode mode) override {
            std::stringstream out;
            ++counter_;
            out << "<span id=\"math_" << counter_ << "\"></span>";
            out << "<script type=\"text/javascript\">";
            out << "katex.render(";
            out << "\"" << EscapeStringForJs(expression) << "\", ";
            out << "document.getElementById(\"math_" << counter_ << "\"), ";
            out << "{displayMode: " << (mode == Mode::Display ? "true" : "false") << ", throwOnError: false}";
            out << ")";
            out << "</script>";

            IMathRenderer::Result result;
            result.ok = true;
            result.html = out.str();
            return result;
        }

    private:
        uint64_t counter_;
    };
}

std::unique_ptr<IMathRenderer> CreateKaTeXInBrowserRenderer() {
    std::unique_ptr<IMathRenderer> ptr;
    ptr.reset(new KaTeXInBrowserRenderer());
    return ptr;
}
