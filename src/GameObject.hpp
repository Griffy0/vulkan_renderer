#pragma once
#include <array>
#include <memory>
#include <stdexcept>
#include "components/Component.hpp"

class GameObject {
    public: 
        uint32_t id;
        std::array<std::unique_ptr<Component>, static_cast<size_t>(ComponentType::COUNT)> components;

        GameObject() {
            static uint32_t id_counter = 0;
            id = id_counter++;
        };

        template <typename T>
        T* getComponent() {
            return static_cast<T*>(components[static_cast<size_t>(T::TYPE)].get());
        };

        template <typename T>
        T* addComponent() {
            if (getComponent<T>())     throw std::runtime_error("Object already has this component!");
            components[static_cast<size_t>(T::TYPE)] = std::make_unique<T>();
            return getComponent<T>();
        };
};
