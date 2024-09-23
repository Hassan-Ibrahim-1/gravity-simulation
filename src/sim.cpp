#include <GLFW/glfw3.h>
#include <sstream>

#include "gravity_object.hpp"
#include "imgui.h"

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

    if (_start) {
        float time = Globals::time_step;
        glm::vec3 acceleration = calculate_acceleration(_planets[1], _planets[0]);
        _planets[1].velocity += acceleration * time;
        // TODO: have update render planets
        update();
        render_planets();
    }
    else {
        trace_predicted_paths(_planets[0], _planets[1]);
    }

    poll_input();
}

void Sim::create_base_imgui_window() {

    ImGui::Begin("gravity simulation");

    ImGui::DragInt("time steps", &_time_steps, 1, 0);

    if (ImGui::Button("reset")) {
        _planets[0].reset(glm::vec3(0));
        _planets[1].reset(glm::vec3(0.3f, 0, 0));
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

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / InputHandler::io->Framerate, InputHandler::io->Framerate);
    ImGui::End();

}

void Sim::create_planet_windows() {
    for (size_t i = 0; i < _planets.size(); i++) {
        if (_selected_planets[i]) {
            GravityObject& planet = _planets[i];
            std::stringstream title;
            title << "Planet " << i;
            bool tmp = _selected_planets[i];
            ImGui::Begin(title.str().c_str(), (bool*)&tmp);
            _selected_planets[i] = tmp;
            ImGui::DragFloat2("position", (float*)&planet.body.transform.position, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat2("scale", (float*)&planet.body.transform.scale, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat("mass", &planet.mass, 100.0f, 0.0f);
            ImGui::DragFloat2("initial velocity", (float*)&planet.initial_velocity, 0.01f, -50.0f, 50.0f);
            ImGui::DragFloat2("current velocity", (float*)&planet.velocity, 0.01f, -50.0f, 50.0f);
            ImGui::ColorEdit3("color", (float*)&planet.body.color);
            ImGui::End();
        }
    }
}

void Sim::poll_input() {
    for (size_t i = 0; i < _planets.size(); i++) {
        // only check for selection if the planet hasn't already been selected
        if (!_selected_planets[i]) {
            if (mouse_click_on_body(_planets[i])) {
                _selected_planets[i] = true;
            }
        }
    }
}

void Sim::create_planet(GravityObject planet) {
    _planets.push_back(planet);
    _selected_planets.push_back(false);
}

void Sim::spawn_initial_planets() {
    create_planet(GravityObject(Circle(Transform(glm::vec3(0), glm::vec3(0.01f, 0.02f, 1)), glm::vec4(1, 0, 0, 1)), 0000000.0f, glm::vec3(0)));
    create_planet(GravityObject(Transform(glm::vec3(0.3, 0, 0), glm::vec3(0.01, 0.02, 1)), 000000.0f));
}

void Sim::update() {
    for (GravityObject& planet : _planets) {
        planet.update();
    }
}

void Sim::render_planets() {
    /*_planets[0].body.render();*/
    for (GravityObject& planet : _planets) {
        planet.body.render();
    }
}

glm::vec3 Sim::calculate_acceleration(GravityObject obj1, GravityObject obj2) {
    glm::vec3 dist = obj2.body.transform.position - obj1.body.transform.position;
    float sqr_dist = glm::length(dist * dist);
    glm::vec3 force_dir = glm::normalize((obj2.body.transform.position - obj1.body.transform.position));
    glm::vec3 acceleration = force_dir * _gravity_constant * obj1.mass * obj2.mass / sqr_dist;
    acceleration /= 1000.0f;
    return acceleration;
}

void Sim::trace_predicted_paths(GravityObject g1, GravityObject g2) {
    float time = Globals::time_step;

    std::vector<Point> positions;

    positions.push_back(Point(g1.body.transform.position, g1.body.color));
    positions.push_back(Point(g2.body.transform.position, g2.body.color));

    for (int i = 0; i < _time_steps; i++) {
        glm::vec3 acceleration = calculate_acceleration(g1, g2);
        /*g1.velocity = g1.initial_velocity + (acceleration) * time;*/
        acceleration = calculate_acceleration(g2, g1);
        g2.velocity += acceleration * time;

        /*g1.update();*/
        g2.update();

        /*positions.push_back(Point(g1.body.transform.position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));*/
        positions.push_back(Point(g2.body.transform.position, glm::vec4(1)));
    }
    for (auto& position : positions) {
        Globals::renderer->draw_point(position);
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

