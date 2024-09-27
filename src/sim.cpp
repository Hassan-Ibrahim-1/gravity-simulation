#include <GLFW/glfw3.h>
#include <cstdio>
#include <imgui.h>

#include <iostream>
#include <sstream>

#include "gravity_object.hpp"
#include "input_handler.hpp"
#include "sim.hpp"
#include "globals.hpp"
#include "utils.hpp"

void Sim::init() {
    spawn_initial_planets();
}

void Sim::run() {
    create_base_imgui_window();
    create_planet_windows();
    create_planet_selection_window();

    if (_start) {
        update();
        render_planets();
    }
    else {
        update_predicted_paths();
        render_predicted_paths();
    }

    poll_input();
}

void Sim::create_base_imgui_window() {
    ImGui::Begin("gravity simulation");

    if (ImGui::Button("open planets menu")) {
        _selection_window_open = true;
    }

    if (ImGui::Button("add planet")) {
        GravityObject& p = create_planet(GravityObject(Circle(Transform(glm::vec3(0), _default_scale),
                                            glm::vec4(1, 1, 1, 1))));
        p.selected = true;
    }

    ImGui::DragInt("time steps", &_time_steps, 1, 0);

    if (ImGui::Button("reset")) {
        for (auto& p : _planets) {
            p.reset();
        }
    }

    if (_start) {
        if (ImGui::Button("stop")) {
            _start = false;
        }
    }

    else if (ImGui::Button("start")) {
        _start = true;
    }

    ImGui::Text("mouse: (%.4f, %.4f)", InputHandler::get_mouse_pos_x(), InputHandler::get_mouse_pos_y());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / InputHandler::io->Framerate, InputHandler::io->Framerate);
    ImGui::End();

}

void Sim::create_planet_windows() {
    for (size_t i = 0; i < _planets.size(); i++) {
        GravityObject& planet = _planets[i];
        if (planet.selected) {
            std::stringstream title;
            title << "Planet " << i + 1;
            ImGui::Begin(title.str().c_str(), &planet.selected);
            ImGui::DragFloat2("position", (float*)&planet.body.transform.position, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat2("scale", (float*)&planet.body.transform.scale, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat("mass", &planet.mass, 100.0f, 0.0f);
            ImGui::DragFloat2("current velocity", (float*)&planet.velocity, 0.001f, -50.0f, 50.0f);
            ImGui::ColorEdit3("color", (float*)&planet.body.color);
            if (ImGui::Button("delete")) {
                _planets.erase(_planets.begin() + i);
            }
            ImGui::End();
        }
    }
}

void Sim::create_planet_selection_window() {
    static int current_item = 0;

    if (_selection_window_open) {
        ImGui::Begin("planets", &_selection_window_open);

        std::vector<std::string> tmp_names;
        std::vector<const char*> planet_names;
        // TODO: Calculate this only when new planets are added
        for (size_t i = 0; i < _planets.size(); i++) {
            std::stringstream ss;
            ss << "Planet " << i + 1;
            tmp_names.push_back(ss.str());
        }

        planet_names.push_back("None");

        for (auto& name : tmp_names) {
            planet_names.push_back(name.c_str());
        }

        /*ImGui::Combo("Planets", &current_item, planet_names, IM_ARRAYSIZE(planet_names));*/
        ImGui::Combo("Planets", &current_item, planet_names.data(), planet_names.size());
        ImGui::End();
        _main_index = current_item - 1;
    }
}

void Sim::poll_input() {
    for (size_t i = 0; i < _planets.size(); i++) {
        // only check for selection if the planet hasn't already been selected
        GravityObject& planet = _planets[i];
        if (!planet.selected) {
            if (mouse_click_on_body(_planets[i])) {
                planet.selected = true;
            }
        }
    }
}

GravityObject& Sim::create_planet(GravityObject planet) {
    planet.selected = true;
    _planets.push_back(planet);
    return _planets.back();
}

void Sim::spawn_initial_planets() {
    create_planet(GravityObject(Circle(Transform(glm::vec3(0), _default_scale),
                                       glm::vec4(1, 0, 0, 1))));
    create_planet(GravityObject(Transform(glm::vec3(0.3, 0, 0), _default_scale)));
}

void Sim::update() {
    glm::vec3 relative_velocity(0);

    if (_main_index != -1) {
        relative_velocity = _planets[_main_index].velocity;
    }
    
    for (GravityObject& planet : _planets) {
        planet.update_velocity(_planets);
        planet.velocity -= relative_velocity;
    }

    for (GravityObject& planet : _planets) {
        planet.update_position();
    }
}

void Sim::render_planets() {
    for (GravityObject& planet : _planets) {
        planet.body.render();
    }
}

void Sim::render_predicted_paths() {
    if (_traced_positions.size() < 2) {
        return;
    }

    for (size_t i = 0; i < _traced_positions.size(); i++) {
        for (size_t j = 0; j < _traced_positions[i].size() - 1; j++) {
            Globals::renderer->draw_line(_traced_positions[i][j], _traced_positions[i][j+1]);
        }
    }

    render_planets();
}

void Sim::update_predicted_paths() {
    _traced_positions.clear();
    std::vector<GravityObject> planets = _planets;

    
    for (size_t i = 0; i < _planets.size(); i++) {
        _traced_positions.push_back(std::vector<Point>());
        _traced_positions[i].push_back(Point(planets[i].body.transform.position, planets[i].body.color));
    }

    for (size_t i = 0; i < _time_steps; i++) {
        glm::vec3 relative_velocity(0);
        if (_main_index != -1) {
            printf("setting relative velocity index: %d\n", _main_index);
            relative_velocity = planets[_main_index].velocity;
        }
        for (auto& planet : planets) {
            planet.update_velocity(planets);
            planet.velocity -= relative_velocity;
        }
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i].update_position();
            _traced_positions[i].push_back(Point(planets[i].body.transform.position, planets[i].body.color));
        }
    }
}

bool Sim::mouse_click_on_body(GravityObject& obj) {
    // NOTE: doesn't work if the if statements are switched for some reason
    if (Utils::mouse_in_circle(obj.body)) {
        if (InputHandler::mouse_button_released(MouseButton::LEFT)) {
            return true;
        }
    }
    return false;
}

