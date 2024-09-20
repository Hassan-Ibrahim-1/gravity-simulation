#pragma once

#include "transform.hpp"

struct Circle {
    Transform transform = Transform(); // transform.position is the center point
    float radius = 1.0f;
    glm::vec4 color = glm::vec4(1);

    Circle(Transform transform = Transform(), float radius = 1.0f, glm::vec4 color = glm::vec4(1));
    Circle(glm::vec3 position, float radius, glm::vec4 color = glm::vec4(1));

};

