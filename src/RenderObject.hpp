#pragma once
#include <glm/glm.hpp>
#include "MeshRegion.hpp"

struct RenderObject {
    const MeshRegion* region;
    glm::mat4 transformation;
};
