#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include "Component.hpp"

class Transform : public Component {
    public: 
        static constexpr ComponentType TYPE = ComponentType::Transform;
        Transform* parent;
        std::vector<Transform*> children;
        glm::vec3 pos = {0.0f, 0.0f, 0.0f}; // Object world pos
        glm::vec3 scale = {1.0f, 1.0f, 1.0f}; // Object scale
        glm::vec3 center = {0.0f, 0.0f, 0.0f}; // Rotation center
        glm::quat rot = {1.0f, 0.0f, 0.0f, 0.0f}; // Rotation quaternion

    glm::mat4 toWorld();
};
