#pragma once

#include <concepts>
enum class ComponentType {
    Transform, 
    Render,
    Camera,
    Physics,
    COUNT
};

class Component{
};

template <typename T>
concept ComponentLike = std::derived_from<T, Component>;

