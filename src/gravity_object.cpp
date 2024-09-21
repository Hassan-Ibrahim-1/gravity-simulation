#include <GLFW/glfw3.h>

#include "gravity_object.hpp"
#include "globals.hpp"

GravityObject::GravityObject(Circle body, float mass, glm::vec3 velocity)
    : body(body), mass(mass), velocity(velocity) {}

void GravityObject::update() {
    glm::vec3 displacement = (((velocity + initial_velocity) / 2.0f) * Globals::delta_time);
    body.transform.position += displacement;
    initial_velocity = velocity;
}

