//#include <SDL3/SDL.h>
#define VOLK_IMPLEMENTATION
#include "VulkanRenderer.hpp"
#include <chrono>
#include <iostream>
#include <cmath>
#include "GameObject.hpp"
#include "components/Component.hpp"
#include "components/Transform.hpp"
#include "components/Render.hpp"
#include "components/Camera.hpp"
#include "Mesh.hpp"

int main() {
    try {
        VulkanRenderer renderer = VulkanRenderer();
        renderer.init();

        bool should_stop = false;
        uint32_t ticks = 0;

        auto startTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime;


        const Mesh mesh {{
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}}},
            {0, 1, 2, 2, 3, 0}};

        MeshRegion region = renderer.addMesh(mesh);

        GameObject obj1;

        obj1.addComponent<Transform>();
        obj1.addComponent<Render>();

        GameObject obj2;

        obj2.addComponent<Transform>();
        obj2.addComponent<Render>();

        obj2.getComponent<Transform>()->parent = obj1.getComponent<Transform>();

        obj1.getComponent<Render>() -> region = &region;
        obj2.getComponent<Render>() -> region = &region;

        obj2.getComponent<Transform>()->pos = {0.0f, 1.0f, -1.0f};
        obj2.getComponent<Transform>()->rotate({0.0f, -0.8f, 0.0f});

        GameObject cam;
        cam.id = 2;

        cam.addComponent<Transform>();
        cam.getComponent<Transform>()->pos = {0.0f, 0.0f, 4.0f};
        cam.addComponent<Camera>();

        renderer.setCamera(cam);

        renderer.addGameObject(obj1);
        renderer.addGameObject(obj2);

        float speed = 0.5f;
        while (!should_stop) {
            currentTime = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration<float>(currentTime - startTime).count();
            startTime = currentTime;

            cam.getComponent<Transform>()->rotate({0.0f, -glm::radians(1.0f*speed), 0.0f});
            double radians = glm::radians((float) ticks*speed);
            cam.getComponent<Transform>()->pos = {std::sin(radians) * 4.0f, 0.0f, std::cos(radians) * 4.0f};

            obj2.getComponent<Transform>()->rotate({0.0f, 0.0f, 1.0f*deltaTime});
            //obj2.getComponent<Transform>()->pos = {0.0f, 0.5f, -0.5f};//std::cos(radians), -0.5f};


            should_stop |= renderer.tick(deltaTime);
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
