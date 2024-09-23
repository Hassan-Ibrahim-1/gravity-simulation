#pragma once

#include "circle.hpp"

class GravityObject {
public:
    Circle body;
    float mass;
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 initial_velocity = glm::vec3(0);

    GravityObject(Circle body, float mass = 500.0f, glm::vec3 initial_velocity = glm::vec3(0));

    // updates scale based on mass
    void update();
    void reset(glm::vec3 position = glm::vec3(0));

private:
    bool _first_update = true;
};

