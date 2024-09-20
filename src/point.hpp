#pragma once

#include <glm/glm.hpp>

#include  "transform.hpp"

struct Point {
    Transform transform = Transform();
    glm::vec4 color = glm::vec4(1);
    
    Point(Transform transform = Transform(), glm::vec4 color = glm::vec4(1));
    Point(glm::vec3 position, glm::vec4 color = glm::vec4(1));
};

