#include <glm/gtc/matrix_transform.hpp>

#include "transform.hpp"

Rotation::Rotation(float yaw, float pitch, float roll)
    : yaw(yaw),
      pitch(pitch),
      roll(roll) {}

Transform::Transform(glm::vec3 position, glm::vec3 scale, Rotation rotation)
    : position(position),
      rotation(rotation),
      scale(scale) {}

glm::mat4 Transform::get_mat4() {
    glm::mat4 mat(1);
    mat = glm::translate(mat, position);
    mat = glm::scale(mat, scale);
    return mat;
}

