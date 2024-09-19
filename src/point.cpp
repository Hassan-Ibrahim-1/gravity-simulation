#include "point.hpp"

Point::Point(Transform transform, glm::vec4 color)
    : transform(transform), color(color) {}

Point::Point(glm::vec3 position, glm::vec4 color)
    : Point(Transform(position), color) {}

