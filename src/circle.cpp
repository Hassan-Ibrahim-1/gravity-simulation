#include "circle.hpp"

Circle::Circle(Transform transform, float radius, glm::vec4 color)
    : transform(transform), radius(radius), color(color) {}

Circle::Circle(glm::vec3 position, float radius, glm::vec4 color)
    : Circle(Transform(position), radius, color) {}

