#pragma once

#include <library/math_renderer/iface/renderer.h>
#include <memory>

std::unique_ptr<IMathRenderer> createDirectRenderer();
