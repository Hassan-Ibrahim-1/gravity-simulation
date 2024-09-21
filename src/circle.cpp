#include "circle.hpp"
#include "globals.hpp"

Circle::Circle(Transform transform, glm::vec4 color)
    : transform(transform), color(color) {}

Circle::Circle(glm::vec3 position, glm::vec4 color)
    : Circle(Transform(position), color) {}

void Circle::render() {
    Globals::renderer->draw_circle(*this);
}

