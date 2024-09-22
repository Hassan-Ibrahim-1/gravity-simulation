#pragma once

#include "circle.hpp"

class GravityObject {
public:
    Circle body;
    float mass;
    glm::vec3 velocity;
    glm::vec3 initial_velocity = glm::vec3(0);

    GravityObject(Circle body, float mass = 500.0f, glm::vec3 velocity = glm::vec3(0));

    // updates scale based on mass
    void update();
private:
    bool _first_update = true;
};

