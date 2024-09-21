#pragma once

#include "transform.hpp"

struct Circle {
    Transform transform = Transform(); // transform.position is the center point
    glm::vec4 color = glm::vec4(1);

    Circle(Transform transform = Transform(), glm::vec4 color = glm::vec4(1));
    Circle(glm::vec3 position, glm::vec4 color = glm::vec4(1));

    void render();
};

