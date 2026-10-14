#pragma once
#include <glm/glm.hpp>
#include "Component.hpp"
#include "Transform.hpp"

class Physics : public Component {
    public:
        static constexpr ComponentType TYPE = ComponentType::Physics;
        Transform* transform = nullptr;
        glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
        glm::vec3 drag_coefficients = {1.0f, 1.0f, 1.0f};
        float mass = 1.0f;
        float gravityMultiplier = 1.0f;

        void addForce(glm::vec3 force);
};
