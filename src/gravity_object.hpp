#pragma once

#include <vector>

#include "circle.hpp"

class GravityObject {
public:
    Circle body;
    float mass;
    glm::vec3 velocity = glm::vec3(0);
    bool selected = false;

    GravityObject(Circle body, float mass = 500.0f, glm::vec3 initial_velocity = glm::vec3(0));

    // updates scale based on mass
    void update_velocity(std::vector<GravityObject>& planets);
    void update_position();
    void reset(glm::vec3 position = glm::vec3(0));

    friend bool operator== (const GravityObject& g1, const GravityObject& g2);
    friend bool operator!= (const GravityObject& g1, const GravityObject& g2);

private:
    static constexpr float _gravity_constant = 0.0000001f;
    /*bool _first_update = true;*/
    glm::vec3 calculate_acceleration(const GravityObject& other);
};

