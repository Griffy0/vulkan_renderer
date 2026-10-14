#include "Transform.hpp"
#include <glm/ext/quaternion_geometric.hpp>

glm::mat4 getTransformation(const Transform& transform) {
    glm::mat4 transformationMatrix = 
        glm::translate(glm::mat4(1.0f), transform.pos)
        * glm::translate(glm::mat4(1.0f), transform.center)
        * glm::mat4_cast(transform.rot)
        * glm::scale(glm::mat4(1.0f), transform.scale)
        * glm::translate(glm::mat4(1.0f), -transform.center);

    if (transform.parent) {
        return getTransformation(*transform.parent) * transformationMatrix;
    };
    return transformationMatrix;
};

glm::mat4 Transform::toWorld() {
    return getTransformation(*this);
};

void Transform::rotate(glm::vec3 v) {
    rot = glm::normalize(rot * glm::quat(-v));
};
