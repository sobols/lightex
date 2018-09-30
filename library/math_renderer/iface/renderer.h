#pragma once

#include <string>

class IMathRenderer {
public:
    enum class Mode {
        Inline,  // $
        Display, // $$
    };

    struct Result {
        bool ok = false;
        std::u16string html;
        std::u16string error;
    };

    virtual ~IMathRenderer() = default;
    virtual Result render(const std::u16string& expression, Mode mode = Mode::Inline) = 0;
};
