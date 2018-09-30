#include "renderer.h"

#include <util/string.h>

#include <sstream>

namespace {
    class DirectRenderer: public IMathRenderer {
    public:
        IMathRenderer::Result Render(const std::string& expression, Mode mode) override {
            std::stringstream out;
            const char* tag = (mode == Mode::Display) ? "div" : "span";
            out << '<' << tag << " class=\"lightex-math\">";
            out << EscapeHtml(expression);
            out << "</" << tag << '>';

            IMathRenderer::Result result;
            result.ok = true;
            result.html = out.str();
            return result;
        }
    };
}

std::unique_ptr<IMathRenderer> CreateKaTeXInBrowserRenderer() {
    std::unique_ptr<IMathRenderer> ptr;
    ptr.reset(new DirectRenderer());
    return ptr;
}
