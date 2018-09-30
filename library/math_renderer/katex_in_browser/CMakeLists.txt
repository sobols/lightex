add_library(math_renderer_katex_in_browser STATIC
    renderer.cpp
)

target_link_libraries(math_renderer_katex_in_browser
    math_renderer_iface
    util
)
