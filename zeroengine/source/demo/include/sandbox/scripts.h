#pragma once

#include "runtime/zero.h"

class CameraController : public Zero::ScriptableEntity {
public:
    void onCreate() override {
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        auto& transform = getComponent<Zero::TransformComponent>().translation;
        float speed     = 3.0f;

        if (Zero::InputSystem::isKeyPressed('A'))
            transform.x -= speed * timestep;
        if (Zero::InputSystem::isKeyPressed('D'))
            transform.x += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('W'))
            transform.y += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('S'))
            transform.y -= speed * timestep;
    }
};

class TexMarchingScript : public Zero::ScriptableEntity {
public:
    void onCreate() override {
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        static float elapsed_time = 0.0f;
        elapsed_time += timestep;
        if (elapsed_time >= 1.0f) {
            auto& sprite = getComponent<Zero::SpriteComponent>();
            sprite.tex_index =
                (sprite.tex_index + 1) % Zero::Application::get().getResourceMngr()->size<Zero::ResourceType::Texture>();
            elapsed_time -= 1.0f;
        }
    }
};
