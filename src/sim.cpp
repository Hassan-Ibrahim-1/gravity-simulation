#include <GLFW/glfw3.h>

#include "globals.hpp"
#include "gravity_object.hpp"
#include "imgui.h"

#include "sim.hpp"

void Sim::init() {
    _start_time = glfwGetTime();
    spawn_initial_planets();
}

void Sim::run() {
    create_imgui_windows();
    // TODO: Treat position like this as well
    _planets[0].body.transform.scale = scale;
    _planets[0].mass = mass1;
    _planets[1].mass = mass2;

    if (_start) {
        glm::vec3 v1 = _v1 / 10.0f;
        /*if (v1 == glm::vec3(0)) {*/
        /*    _planets[0].velocity = v1;*/
        /*}*/
        /*else {*/
        glm::vec3 acceleration = calculate_acceleration(_planets[0], _planets[1]);
        if (acceleration.x == 0 && acceleration.y == 0) {
            printf("acceleration is 0");
        }
        acceleration /= 10000.0f;
        _planets[0].velocity = v1 + (acceleration) * Globals::delta_time;
        /*}*/
        // TODO: have update render planets
        update();
    }

    _start_time = glfwGetTime();
    render_planets();
}

void Sim::create_imgui_windows() {
    ImGui::Begin("gravity simulation");
    /*ImGui::DragFloat("time offset", &_time_offset, 0.01f, -1.0f, 2.0f);*/
    ImGui::DragFloat("mass1", &mass1, 1.0f, 0.0f);
    ImGui::DragFloat2("velocity1", (float*)&_v1, 0.1f, -50.0f, 50.0f);
    ImGui::DragFloat2("scale1", (float*)&scale, 0.01f, -1.0f, 1.0f);

    ImGui::DragFloat("mass2", &mass2, 1.0f, 0.0f);
    ImGui::DragFloat2("position 2", (float*) &_planets[1].body.transform.position, 0.01f, -1.0f, 1.0f);

    /*ImGui::DragFloat("gravity", &_gravity_constant, 0.0000001f);*/


    if (ImGui::Button("reset")) {
        _planets[0].body.transform.position = glm::vec3(0);
    }

    if (_start) {
        _start = ImGui::Button("stop");
    }

    else if (!_start) {
        _start = ImGui::Button("start");
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Globals::io->Framerate, Globals::io->Framerate);
    ImGui::End();

}

void Sim::spawn_initial_planets() {
    GravityObject g1 = GravityObject(Transform(glm::vec3(0), scale), 1.0f, _v1);
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

glm::vec3 Sim::calculate_force(float mass1, float mass2, glm::vec3 distance) {
    return _gravity_constant * ((mass1 * mass1) / distance);
}

glm::vec3 Sim::calculate_acceleration(GravityObject obj1, GravityObject obj2) {
    // a = f / m
    return calculate_force(obj1.mass, obj2.mass, obj2.body.transform.position - obj1.body.transform.position);
}

