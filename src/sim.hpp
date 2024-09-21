#pragma once

#include <vector>

#include "gravity_object.hpp"

class Sim {
public:
    void init();
    void run();
    /*void end();*/

private:
    std::vector<GravityObject> _planets;
    glm::vec3 _v1 = glm::vec3(0.001f, 0.01f, 0.0f);

    float _start_time = 0.0f;
    float _time_offset = 0.0f;

    void create_imgui_windows();
    void spawn_initial_planets();

    void update();
    void render_planets();
};

