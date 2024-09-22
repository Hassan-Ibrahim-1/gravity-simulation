#include <GLFW/glfw3.h>

#include "gravity_object.hpp"
#include "globals.hpp"

GravityObject::GravityObject(Circle body, float mass, glm::vec3 velocity)
    : body(body), mass(mass), initial_velocity(velocity) {}

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

