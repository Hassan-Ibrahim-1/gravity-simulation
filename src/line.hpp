#pragma once

#include <glm/glm.hpp>

#include "point.hpp"

struct Line {
    Point p1;
    Point p2;
    glm::vec4 color;

    Line(Point p1, Point p2, glm::vec4 color = glm::vec4(1));
};

