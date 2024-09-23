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
    // these bools control whether or not a planet window is shown for the specific planet
    // each index corresponds to a planet in _planets
    std::vector<bool> _selected_planets;
    /*float mass1 = 20000.0f;*/
    /*float mass2 = 10000.0f;*/

    static constexpr float _gravity_constant = 0.0000001f;

    bool _start = false;

    int _time_steps = 500;

    void create_base_imgui_window();
    void create_planet(GravityObject planet);
    void spawn_initial_planets();
    // Creates planet windows 
    void create_planet_windows();
    void poll_input(); // checks if any planet has been selected

    void update();
    void render_planets();

    // calcuates the acceleration to applied to obj1
    glm::vec3 calculate_acceleration(GravityObject obj1, GravityObject obj2);

    // TODO: only calculate if values are changed
    void trace_predicted_paths(GravityObject obj1, GravityObject obj2);
    bool mouse_click_on_body(GravityObject& obj);
};

