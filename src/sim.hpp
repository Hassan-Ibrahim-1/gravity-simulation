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
    float mass1 = 20000.0f;
    float mass2 = 10000.0f;

    static constexpr float _gravity_constant = 0.0000001f;

    bool _start = false;

    int _time_steps = 500;

    void create_imgui_windows();
    void spawn_initial_planets();

    void update();
    void render_planets();

    // calcuates the acceleration to applied to obj1
    glm::vec3 calculate_acceleration(GravityObject obj1, GravityObject obj2);

    // TODO: only calculate if values are changed
    void trace_predicted_paths(GravityObject obj1, GravityObject obj2);
};

