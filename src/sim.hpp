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
    glm::vec3 _v1 = glm::vec3(0);
    glm::vec3 scale = glm::vec3(0.04f, 0.08f, 0.0f);
    float mass1 = 1000000.0f;
    float mass2 = 2000000.0f;

    float _gravity_constant = 0.0000007f;

    float _start_time = 0.0f;

    bool _start = false;

    void create_imgui_windows();
    void spawn_initial_planets();

    void update();
    void render_planets();

    glm::vec3 calculate_force(float mass1, float mass2, glm::vec3 distance);
    // calcuates the acceleration to applied to obj1
    glm::vec3 calculate_acceleration(GravityObject obj1, GravityObject obj2);
};

