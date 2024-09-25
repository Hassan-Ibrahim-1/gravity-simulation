#include <GLFW/glfw3.h>
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
    /*create_planet_selection_window();*/

    if (_main_planet == nullptr) {
    
    }

    if (_start) {
        /*float time = Globals::time_step;*/
        /*glm::vec3 acceleration = calculate_acceleration(_planets[1], _planets[0]);*/
        /*_planets[1].velocity += acceleration * time;*/
        /*acceleration = calculate_acceleration(_planets[0], _planets[1]);*/
        /*_planets[0].velocity += acceleration * time;*/
        // TODO: have update render planets
        update();
        render_planets();
    }
    else {
        update_predicted_paths();
        render_predicted_paths();
        /*trace_predicted_paths();*/
    }

    poll_input();
}

void Sim::create_base_imgui_window() {
    ImGui::Begin("gravity simulation");

    if (ImGui::Button("Add planet")) {
        GravityObject& p = create_planet(GravityObject(Circle(Transform(glm::vec3(0), _default_scale),
                                            glm::vec4(1, 0, 0, 1)), 0.0f, glm::vec3(0)));
        p.selected = true;
    }

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

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / InputHandler::io->Framerate, InputHandler::io->Framerate);
    ImGui::End();

}

void Sim::create_planet_windows() {
    for (size_t i = 0; i < _planets.size(); i++) {
        GravityObject& planet = _planets[i];
        if (planet.selected) {
            std::stringstream title;
            title << "Planet " << i;
            ImGui::Begin(title.str().c_str(), &planet.selected);
            ImGui::DragFloat2("position", (float*)&planet.body.transform.position, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat2("scale", (float*)&planet.body.transform.scale, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat("mass", &planet.mass, 100.0f, 0.0f);
            /*ImGui::DragFloat2("initial velocity", (float*)&planet.initial_velocity, 0.01f, -50.0f, 50.0f);*/
            /*glm::vec3 tmpv = planet.velocity;*/
            ImGui::DragFloat2("current velocity", (float*)&planet.velocity, 0.001f, -50.0f, 50.0f);
            /*planet.velocity = tmpv / 100.0f;*/
            ImGui::ColorEdit3("color", (float*)&planet.body.color);
            ImGui::End();
        }
    }
}

void Sim::create_planet_selection_window() {
    static int current_item = 0;

    ImGui::Begin("planets", &_selection_window_open);
    /*printf("%s\n", ss.str().c_str());*/
    /*std::cout << ss.str() << '\n';*/
    /*std::string test = ss.str();*/
    /*planet_names[i] = test.c_str();*/

    std::vector<std::string> tmp_names;
    /*const char* planet_names[_planets.size()];*/
    std::vector<const char*> planet_names;
    // TODO: Calculate this only when new planets are added
    for (size_t i = 0; i < _planets.size(); i++) {
        std::stringstream ss;
        ss << "Planet " << i + 1;
        tmp_names.push_back(ss.str());
    }
    
    for (auto& name : tmp_names) {
        planet_names.push_back(name.c_str());
    }

    /*ImGui::Combo("Planets", &current_item, planet_names, IM_ARRAYSIZE(planet_names));*/
    ImGui::Combo("Planets", &current_item, planet_names.data(), planet_names.size());
    ImGui::End();
    _main_planet = &_planets[current_item];
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
    _planets.push_back(planet);
    return _planets.back();
}

void Sim::spawn_initial_planets() {
    create_planet(GravityObject(Circle(Transform(glm::vec3(0), _default_scale),
                                       glm::vec4(1, 0, 0, 1)), 0000000.0f, glm::vec3(0)));
    create_planet(GravityObject(Transform(glm::vec3(0.3, 0, 0), _default_scale), 000000.0f));
}

void Sim::update() {
    for (GravityObject& planet : _planets) {
        planet.update_velocity(_planets);
    }
}

void Sim::render_planets() {
    /*_planets[0].body.render();*/
    for (GravityObject& planet : _planets) {
        planet.body.render();
    }
}

void Sim::render_predicted_paths() {
    for (auto& position : _traced_positions) {
        Globals::renderer->draw_point(position);
    }
}

void Sim::update_predicted_paths() {
    _traced_positions.clear();
    float time = Globals::time_step;

    std::vector<GravityObject> planets = _planets;
    
    for (const auto& planet : planets) {
        _traced_positions.push_back(Point(planet.body.transform.position, planet.body.color));
    }
    /*_traced_positions.push_back(Point(g1.body.transform.position, g1.body.color));*/
    /*_traced_positions.push_back(Point(g2.body.transform.position, g2.body.color));*/

    for (int i = 0; i < _time_steps; i++) {
        for (auto& planet : planets) {
            planet.update_velocity(planets);
        }
        for (auto& planet : planets) {
            planet.update_position();
            _traced_positions.push_back(Point(planet.body.transform.position, planet.body.color));
        }
        /*glm::vec3 acceleration = calculate_acceleration(g1, g2);*/
        /*g1.velocity += (acceleration) * time;*/
        /*acceleration = calculate_acceleration(g2, g1);*/
        /*g2.velocity += acceleration * time;*/
        /**/
        /*g1.update_velocity();*/
        /*g2.update_velocity();*/
        /**/
        /*_traced_positions.push_back(Point(g1.body.transform.position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));*/
        /*_traced_positions.push_back(Point(g2.body.transform.position, glm::vec4(1)));*/
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

