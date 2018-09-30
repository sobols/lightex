#include "renderer.h"

#include <util/string.h>

#include <sstream>

namespace {
    class DirectRenderer: public IMathRenderer {
    public:
        IMathRenderer::Result render(const std::u16string& expression, Mode mode) override {
            std::basic_stringstream<char16_t> out;
            const char16_t* tag = (mode == Mode::Display) ? u"div" : u"span";
            out << u'<' << tag << u" class=\"lightex-math>";
            out << escapeHtml(expression);
            out << u'<' << u'/' << tag << u'>';

            IMathRenderer::Result result;
            result.ok = true;
            result.html = out.str();
            return result;
        }
    };
}

std::unique_ptr<IMathRenderer> createKaTeXInBrowserRenderer() {
    std::unique_ptr<IMathRenderer> ptr;
    ptr.reset(new DirectRenderer());
    return ptr;
}
