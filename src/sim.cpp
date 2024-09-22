#include <GLFW/glfw3.h>

#include "imgui.h"

#include "sim.hpp"
#include "globals.hpp"

void Sim::init() {
    spawn_initial_planets();
}

void Sim::run() {
    create_imgui_windows();
    // TODO: Treat position like this as well
    _planets[0].mass = mass1;
    _planets[1].mass = mass2;

    if (_start) {
        glm::vec3 acceleration = calculate_acceleration(_planets[0], _planets[1]);
        float time = Globals::time_step;
        acceleration = calculate_acceleration(_planets[1], _planets[0]);
        _planets[1].velocity += acceleration * time;
        // TODO: have update render planets
        update();
        render_planets();
    }

    else {
        trace_predicted_paths(_planets[0], _planets[1]);
    }

}

void Sim::create_imgui_windows() {
    ImGui::Begin("gravity simulation");
    ImGui::DragFloat("mass1", &mass1, 100.0f, 0.0f);
    /*ImGui::DragFloat2("scale1", (float*)&scale, 0.01f, -1.0f, 1.0f);*/

    ImGui::DragFloat("mass2", &mass2, 100.0f, 0.0f);
    ImGui::DragFloat2("position 2", (float*) &_planets[1].body.transform.position, 0.01f, -1.0f, 1.0f);
    ImGui::DragFloat2("velocity1", (float*)&_planets[1].initial_velocity, 0.01f, -50.0f, 50.0f);
    /*ImGui::DragFloat2("scale2", (float*)&_planets[1].body.transform.scale, 0.01f, -1.0f, 1.0f);*/

    ImGui::DragInt("time steps", &_time_steps, 1, 0);

    if (ImGui::Button("reset")) {
        _planets[0].body.transform.position = glm::vec3(0);
        _planets[1].body.transform.position = glm::vec3(0.3f, 0, 0);
        _planets[0].initial_velocity = glm::vec3(0);
        _planets[1].initial_velocity = glm::vec3(0);
    }

    if (_start) {
        if (ImGui::Button("stop")) {
            _start = false;
        }
    }

    else if (ImGui::Button("start")) {
        _start = true;
    }

    ImGui::Text("mouse: (%.4f, %.4f)", Globals::mouse_pos.x, Globals::mouse_pos.y);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Globals::io->Framerate, Globals::io->Framerate);
    ImGui::End();

}

void Sim::spawn_initial_planets() {
    GravityObject g1 = GravityObject(Transform(glm::vec3(0), glm::vec3(0.01f, 0.02f, 1)), 1.0f, glm::vec3(0));
    GravityObject g2 = GravityObject(Transform(glm::vec3(0.3, 0, 0), glm::vec3(0.01, 0.02, 1)));
    _planets.push_back(g1);
    _planets.push_back(g2);
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

    positions.push_back(Point(g2.body.transform.position, glm::vec4(1)));

    for (int i = 0; i < _time_steps; i++) {
        glm::vec3 acceleration = calculate_acceleration(g1, g2);
        /*g1.velocity = g1.initial_velocity + (acceleration) * time;*/
        acceleration = calculate_acceleration(g2, g1);
        g2.velocity += acceleration * time;

        /*g1.update();*/
        g2.update();

        positions.push_back(Point(g1.body.transform.position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
        positions.push_back(Point(g2.body.transform.position, glm::vec4(1)));
    }
    for (auto& position : positions) {
        Globals::renderer->draw_point(position);
    }
}

