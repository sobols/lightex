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
        std::string html;
        std::string error;
    };

    virtual ~IMathRenderer() = default;
    virtual Result Render(const std::string& expression, Mode mode = Mode::Inline) = 0;
};
