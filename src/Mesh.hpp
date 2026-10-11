#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
};
