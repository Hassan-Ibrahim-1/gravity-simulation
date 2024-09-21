#include <GLFW/glfw3.h>

#include "globals.hpp"
#include "imgui.h"

#include "sim.hpp"

void Sim::init() {
    _start_time = glfwGetTime();
    spawn_initial_planets();
}

void Sim::run() {
    create_imgui_windows();
    // TODO: Treat position like this as well
    glm::vec3 v1 = _v1 / 10.0f;
    _planets[0].velocity = v1 + 1.0f *  Globals::delta_time;
    if (_start_time + _time_offset < glfwGetTime()) {
        update();
        _start_time = glfwGetTime();
    }
    render_planets();
}

void Sim::create_imgui_windows() {
    ImGui::Begin("gravity simulation");
    ImGui::DragFloat("time offset", &_time_offset, 0.01f, -1.0f, 2.0f);
    ImGui::DragFloat3("velocity", (float*)&_v1, 0.1f, -50.0f, 50.0f);
    if (ImGui::Button("reset")) {
        _planets[0].body.transform.position = glm::vec3(0);
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Globals::io->Framerate, Globals::io->Framerate);
    ImGui::End();

}

void Sim::spawn_initial_planets() {
    GravityObject g1 = GravityObject(Transform(glm::vec3(0), glm::vec3(0.1, 0.23, 1)), 1.0f, _v1);
    GravityObject g2 = GravityObject(Transform(glm::vec3(0.3), glm::vec3(0.2, 0.33, 1)));
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


