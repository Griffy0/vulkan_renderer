#pragma once
#include <volk.h>
#include <glm/glm.hpp>
#include <array>

struct Vertex {
    alignas(16) glm::vec3 pos;
    alignas(8) glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};
