#pragma once

#include "runtime/zero.h"
class CameraController : public Zero::ScriptableEntity {
public:
    void onCreate() override {
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        auto& transform = getComponent<Zero::TransformComponent>();
        float speed     = 3.0f;

        // if (Zero::InputSystem::isKeyPressed('A'))
        //     transform.translation.x -= speed * timestep;
        // if (Zero::InputSystem::isKeyPressed('D'))
        //     transform.translation.x += speed * timestep;
        // if (Zero::InputSystem::isKeyPressed('W'))
        //     transform.translation.y += speed * timestep;
        // if (Zero::InputSystem::isKeyPressed('S'))
        //     transform.translation.y -= speed * timestep;
    }
};

class AnimeScript : public Zero::ScriptableEntity {
    void onCreate() override {
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        auto& transform = getComponent<Zero::TransformComponent>();
        float speed     = 1.0f;

        if (Zero::InputSystem::isKeyPressed('A'))
            transform.translation.x -= speed * timestep;
        if (Zero::InputSystem::isKeyPressed('D'))
            transform.translation.x += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('W'))
            transform.translation.y += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('S'))
            transform.translation.y -= speed * timestep;

        static int bias = 18;

        if (Zero::InputSystem::isKeyPressed('W'))
            bias = 6;
        if (Zero::InputSystem::isKeyPressed('S'))
            bias = 18;
        if (Zero::InputSystem::isKeyPressed('D'))
            bias = 0;
        if (Zero::InputSystem::isKeyPressed('A'))
            bias = 12;

        static uint32_t x_pos        = 0;
        static float    elapsed_time = 0.0f;
        elapsed_time += timestep;
        if (elapsed_time >= 0.1f) {
            auto& tile   = getComponent<Zero::TileComponent>();
            tile.coord_x = ((tile.coord_x + 1) % 6) + bias;
            elapsed_time -= 0.1f;
        }
    }
};