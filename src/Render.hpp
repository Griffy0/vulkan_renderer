#pragma once
#include "Component.hpp"
#include "MeshRegion.hpp"

class Render : public Component {
    public:
        static constexpr ComponentType TYPE = ComponentType::Render;
        bool visible = true;
        MeshRegion* region;
        // Texture reference
};
