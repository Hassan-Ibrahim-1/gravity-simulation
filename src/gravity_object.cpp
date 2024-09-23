#include <GLFW/glfw3.h>

#include "gravity_object.hpp"
#include "globals.hpp"

GravityObject::GravityObject(Circle body, float mass, glm::vec3 initial_velocity)
    : body(body), mass(mass), initial_velocity(initial_velocity) {}

void GravityObject::update() {
    float time = Globals::time_step;
    if (_first_update) {
        initial_velocity /= 100.0f;
        velocity = initial_velocity;
        body.transform.position += velocity;
        _first_update = false;
        return;
    }
    body.transform.position += velocity * time;
}

void GravityObject::reset(glm::vec3 position) {
    body.transform.position = glm::vec3(0);
    velocity = glm::vec3(0);
    initial_velocity = glm::vec3(0);
    _first_update = true;
}

