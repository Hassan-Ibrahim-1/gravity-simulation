#include <GLFW/glfw3.h>

#include "gravity_object.hpp"
#include "globals.hpp"
#include "sim.hpp"

GravityObject::GravityObject(Circle body, float mass, glm::vec3 initial_velocity)
    : body(body), mass(mass), velocity(initial_velocity) {}

void GravityObject::update_velocity(std::vector<GravityObject>& planets) {
    float time = Globals::time_step;

    /*if (_first_update) {*/
    /*    initial_velocity /= 100.0f;*/
    /*    velocity = initial_velocity;*/
    /*    body.transform.position += velocity * time;*/
    /*    _first_update = false;*/
    /*    return;*/
    /*}*/

    for (const GravityObject& planet : planets) {
        if (planet == *this) {
            continue;
        }
        glm::vec3 acceleration = calculate_acceleration(planet);
        velocity += acceleration * time;
    }

    body.transform.position += velocity * time;
}

void GravityObject::update_position() {
    body.transform.position += velocity * Globals::time_step;
}

void GravityObject::reset(glm::vec3 position) {
    body.transform.position = glm::vec3(0);
    velocity = glm::vec3(0);
    /*initial_velocity = glm::vec3(0);*/
    /*_first_update = true;*/
}

glm::vec3 GravityObject::calculate_acceleration(const GravityObject& other) {
    glm::vec3 dist = other.body.transform.position - this->body.transform.position;
    float sqr_dist = glm::length(dist * dist);
    glm::vec3 force_dir = glm::normalize((other.body.transform.position - this->body.transform.position));
    glm::vec3 acceleration = force_dir * _gravity_constant * this->mass * other.mass / sqr_dist;
    acceleration /= 1000.0f;
    return acceleration;
}

bool operator==(const GravityObject& g1, const GravityObject& g2) {
    return (g1.body == g2.body)
        && (g1.mass == g2.mass)
        && (g1.velocity == g2.velocity);
}

bool operator!=(const GravityObject& g1, const GravityObject& g2) {
    return (g1.body != g2.body)
        || (g1.mass != g2.mass)
        || (g1.velocity != g2.velocity);
}

