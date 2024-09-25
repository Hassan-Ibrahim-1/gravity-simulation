#pragma once

#include <vector>

#include "gravity_object.hpp"
#include "point.hpp"

class Sim {
public:
    void init();
    void run();
    /*void end();*/

private:
    std::vector<GravityObject> _planets;
    std::vector<Point> _traced_positions;
    // planet used as a reference for relative velocity
    GravityObject* _main_planet = nullptr;
    /*float mass1 = 20000.0f;*/
    /*float mass2 = 10000.0f;*/

    static constexpr glm::vec3 _default_scale = glm::vec3(0.01, 0.02, 1);

    bool _start = false;
    int _time_steps = 500;

    bool _selection_window_open = true;

    void create_base_imgui_window();
    void create_planet_windows();
    void create_planet_selection_window();

    GravityObject& create_planet(GravityObject planet);
    void spawn_initial_planets();
    void poll_input(); // checks if any planet has been selected

    void update();
    void render_planets();

    void render_predicted_paths();
    void update_predicted_paths();

    bool mouse_click_on_body(GravityObject& obj);
};

