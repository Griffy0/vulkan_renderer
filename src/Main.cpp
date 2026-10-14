#define VOLK_IMPLEMENTATION
#include "VulkanRenderer.hpp"
#include "PhysicsHandler.hpp"
#include "RandomGenerator.hpp"
#include <SDL3/SDL.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include "GameObject.hpp"
#include "components/Component.hpp"
#include "components/Transform.hpp"
#include "components/Render.hpp"
#include "components/Camera.hpp"
#include "components/Physics.hpp"
#include "Mesh.hpp"

int main() {
    try {
        VulkanRenderer renderer = VulkanRenderer();
        SDL_Window* window = renderer.init();
        SDL_SetWindowRelativeMouseMode(window, true);

        PhysicsHandler physHandler = PhysicsHandler();
        physHandler.init();

        bool should_stop = false;
        uint32_t ticks = 0;

        auto startTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime;

        float tx = 0.5f / 54.0f;
        float ty = 0.5f / 36.0f;
        const Mesh quad {{
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}}},
            {0, 1, 2, 2, 3, 0}};

        const Mesh cube {{
            // FRONT
            {{-0.5f, -0.5f, 0.5f}, {1.0f/3.0f+tx, 1.0f-ty}},
            {{0.5f, -0.5f, 0.5f}, {2.0f/3.0f-tx, 1.0f-ty}},
            {{0.5f, 0.5f, 0.5f}, {2.0f/3.0f-tx, 0.5f+ty}},
            {{-0.5f, 0.5f, 0.5f}, {1.0f/3.0f+tx, 0.5f+ty}},

            // RIGHT
            {{0.5f, -0.5f, 0.5f}, {2.0f/3.0f+tx, 1.0f-ty}},
            {{0.5f, -0.5f, -0.5f}, {1.0f-tx, 1.0f-ty}},
            {{0.5f, 0.5f, -0.5f}, {1.0f-tx, 0.5f+ty}},
            {{0.5f, 0.5f, 0.5f}, {2.0f/3.0f+tx, 0.5f+ty}},
            
            // LEFT
            {{-0.5f, -0.5f, -0.5f}, {0.0f+tx, 1.0f-ty}},
            {{-0.5f, -0.5f, 0.5f}, {1.0f/3.0f-tx, 1.0f-ty}},
            {{-0.5f, 0.5f, 0.5f}, {1.0f/3.0f-tx, 0.5f+ty}},
            {{-0.5f, 0.5f, -0.5f}, {0.0f+tx, 0.5f+ty}},

            // BACK
            {{0.5f, -0.5f, -0.5f}, {0.0f+tx, 0.5f-ty}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f/3.0f-tx, 0.5f-ty}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f/3.0f-tx, 0.0f+ty}},
            {{0.5f, 0.5f, -0.5f}, {0.0f+tx, 0.0f+ty}},

            // BOTTOM
            {{-0.5f, -0.5f, -0.5f}, {2.0f/3.0f+tx, 0.5f-ty}},
            {{0.5f, -0.5f, -0.5f}, {1.0f-tx, 0.5f-ty}},
            {{0.5f, -0.5f, 0.5f}, {1.0f-tx, 0.0f+ty}},
            {{-0.5f, -0.5f, 0.5f}, {2.0f/3.0f+tx, 0.0f+ty}},

            // TOP
            {{-0.5f, 0.5f, 0.5f}, {1.0f/3.0f+tx, 0.5f-ty}},
            {{0.5f, 0.5f, 0.5f}, {2.0f/3.0f-tx, 0.5f-ty}},
            {{0.5f, 0.5f, -0.5f}, {2.0f/3.0f-tx, 0.0f+ty}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f/3.0f+tx, 0.0f+ty}}},

            {0, 1, 2, 2, 3, 0,          // FRONT
             4, 5, 6, 6, 7, 4,          // RIGHT
             8, 9, 10, 10, 11, 8,       // LEFT
             12, 13, 14, 14, 15, 12,    // BACK
             16, 17, 18, 18, 19, 16,    // BOTTOM
             20, 21, 22, 22, 23, 20     // TOP             
            }};

        MeshRegion quadRegion = renderer.addMesh(quad);
        MeshRegion cubeRegion = renderer.addMesh(cube);

        GameObject obj1;

        obj1.addComponent<Transform>();
        obj1.addComponent<Render>();
        obj1.addComponent<Physics>();

        GameObject obj2;

        obj2.addComponent<Transform>();
        obj2.addComponent<Render>();
        obj2.addComponent<Physics>();

        obj2.getComponent<Transform>()->parent = obj1.getComponent<Transform>();

        obj1.getComponent<Render>()->region = &cubeRegion;
        obj2.getComponent<Render>()->region = &quadRegion;

        obj2.getComponent<Transform>()->pos = {0.0f, 1.0f, -1.0f};
        obj2.getComponent<Transform>()->rotate({0.0f, -0.8f, 0.0f});

        GameObject cam;

        cam.addComponent<Transform>();
        cam.getComponent<Transform>()->pos = {0.0f, 1.0f, 4.0f};
        cam.addComponent<Camera>();
        //cam.getComponent<Camera>()->fov = 20.0f;
        cam.addComponent<Physics>();

        renderer.setCamera(cam);

        renderer.addGameObject(obj1);
        renderer.addGameObject(obj2);

        physHandler.addGameObject(&obj1);
        physHandler.addGameObject(&obj2);
        physHandler.addGameObject(&cam);

        RandomGenerator rng;

        float deltaMouseX = 0.0f;
        float deltaMouseY = 0.0f;
        float xSensitivity = 0.25f;
        float ySensitivity = 0.25f;
        float speed = 0.5f;
        cam.getComponent<Physics>()->addForce({-2.0f, 0.0f, 0.0f});
        while (!should_stop) {
            currentTime = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration<float>(currentTime - startTime).count();
            startTime = currentTime;
            SDL_GetRelativeMouseState(&deltaMouseX, &deltaMouseY);
            glm::quat& camera_rot = cam.getComponent<Transform>()->rot;
            camera_rot = 
                glm::angleAxis(glm::radians(-deltaMouseX * xSensitivity), glm::vec3(0, 1, 0))
                * camera_rot
                * glm::angleAxis(glm::radians(-deltaMouseY * ySensitivity), glm::vec3(1, 0, 0));

            //cam.getComponent<Transform>()->rotate({0.0f, -glm::radians(1.0f*speed), 0.0f});
            double radians = glm::radians((float) ticks*speed);

            //obj1.getComponent<Transform>()->rotate({0.0f, 1.0f * deltaTime, 0.0f});
            //obj2.getComponent<Transform>()->rotate({0.0f, 0.0f, 1.0f*deltaTime});
            //obj2.getComponent<Transform>()->pos = {0.0f, 0.5f, -0.5f};//std::cos(radians), -0.5f};
            Physics phys = *cam.getComponent<Physics>();
            //std::cout << std::format("Velocity; x={}, y={}, z={}", phys.velocity.x, phys.velocity.y, phys.velocity.z) << std::endl;
            std::cout << std::format("FPS: {}", 1/deltaTime) << std::endl;
            should_stop |= renderer.tick(deltaTime);
            should_stop |= physHandler.tick(deltaTime);
            ticks++;
        };
        renderer.end();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Program Closing" << std::endl;
    return EXIT_SUCCESS;
};
