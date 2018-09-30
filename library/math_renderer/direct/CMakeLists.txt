add_library(math_renderer_direct STATIC
    renderer.cpp
)

target_link_libraries(math_renderer_direct
    math_renderer_iface
    util
)
