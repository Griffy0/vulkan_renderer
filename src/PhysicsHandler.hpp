#pragma once
#include <vector>
#include "GameObject.hpp"

class PhysicsHandler {
    public:
        float gravity = 9.8;

        void init();
        bool tick(float deltaTime);
        void end();
        void addGameObject(GameObject* gameObject);
        void updateGameObject(GameObject* gameObject, float deltaTime);

    private:
        std::vector<GameObject*> gameObjects;
};
