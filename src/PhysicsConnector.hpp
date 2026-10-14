#pragma once
#include "GameObject.hpp"

class PhysicsConnector {
    public:
        bool rigid = true;
        GameObject& a;
        GameObject& b;
};
