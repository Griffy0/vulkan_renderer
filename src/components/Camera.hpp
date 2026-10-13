#pragma once
#include <glm/glm.hpp>
#include "Component.hpp"

class Camera : public Component {
    public:
        static constexpr ComponentType TYPE = ComponentType::Camera;
        float fov = 80.0f;
        float near_plane = 0.1f;
        float far_plane = 10.0f;
};
