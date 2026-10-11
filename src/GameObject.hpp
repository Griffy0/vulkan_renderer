#pragma once
#include <array>
#include <memory>
#include "components/Component.hpp"

class GameObject {
    public: 
        uint32_t id;
        std::array<std::unique_ptr<Component>, static_cast<size_t>(ComponentType::COUNT)> components;

    template <typename T>

    T* getComponent() {
        return static_cast<T*>(components[static_cast<size_t>(T::TYPE)].get());
    };
};
