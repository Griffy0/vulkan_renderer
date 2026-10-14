#include "PhysicsHandler.hpp"
#include "components/Physics.hpp"
#include "components/Transform.hpp"
#include <stdexcept>

void PhysicsHandler::init(){

};

bool PhysicsHandler::tick(float deltaTime) {
    for (GameObject* gameObject : gameObjects) {
        updateGameObject(gameObject, deltaTime);
    };
    return false;
};

void PhysicsHandler::end() {

};

void PhysicsHandler::addGameObject(GameObject* gameObject) {
    if (!gameObject->getComponent<Physics>())   throw std::runtime_error("No physics component attached to gameobject!");
    if (!gameObject->getComponent<Transform>()) throw std::runtime_error("No transform component attached to gameobject!");
    gameObjects.push_back(gameObject);
};

void PhysicsHandler::updateGameObject(GameObject* gameObject, float deltaTime) {
    Physics* phys = gameObject->getComponent<Physics>();
    Transform* transform = gameObject->getComponent<Transform>();

    phys->velocity -= (phys->drag_coefficients * 1.225f * 0.5f * phys->velocity * phys->velocity * glm::sign(phys->velocity)) / phys->mass * deltaTime; // Fd = 1/2 Cd d v^2 a
    phys->velocity += phys->acceleration * deltaTime;
    transform->pos += phys->velocity * deltaTime;
};
