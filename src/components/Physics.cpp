#include "Physics.hpp"

void Physics::addForce(glm::vec3 force) {
    velocity += force / mass ;
};
