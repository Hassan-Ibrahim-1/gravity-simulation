#pragma once

#include "imgui.h"

#include "camera.hpp"
#include "renderer.hpp"

namespace Globals {

inline Camera camera;
inline ImGuiIO* io = nullptr;
inline Renderer* renderer;
inline glm::vec2 mouse_pos = glm::vec2(0);
inline float delta_time = 0.0f;
inline float time_step = 0.01f;

}

