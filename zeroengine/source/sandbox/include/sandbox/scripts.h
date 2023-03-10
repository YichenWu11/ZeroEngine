#pragma once

#include "runtime/zero.h"
extern int* g_player_money;
class CameraController : public Zero::ScriptableEntity {
public:
    void onCreate() override {
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        auto& transform = getComponent<Zero::TransformComponent>();
        float speed     = 3.0f;

        if (Zero::InputSystem::isKeyPressed('A'))
            transform.translation.x -= speed * timestep;
        if (Zero::InputSystem::isKeyPressed('D'))
            transform.translation.x += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('W'))
            transform.translation.y += speed * timestep;
        if (Zero::InputSystem::isKeyPressed('S'))
            transform.translation.y -= speed * timestep;
    }
};

class PlayerController : public Zero::ScriptableEntity {
public:
    void onCreate() override {
        g_player_money = &money;
    }

    void onDestroy() override {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        auto& rigidbody2d = getComponent<Zero::Rigidbody2DComponent>();
        float speed       = 3.0f;

        if (Zero::InputSystem::isKeyPressed('A'))
            rigidbody2d.linear_velocity.x = -speed;
        if (Zero::InputSystem::isKeyPressed('D'))
            rigidbody2d.linear_velocity.x = speed;
        if (Zero::InputSystem::isKeyPressed('W'))
            rigidbody2d.linear_velocity.y = speed;
        if (Zero::InputSystem::isKeyPressed('S'))
            rigidbody2d.linear_velocity.y = -speed;
    }

    int money{0};

    void onCollisionEnter(Zero::Entity entity) override {
        if (entity.hasComponent<Zero::NameComponent>() && entity.hasComponent<Zero::TransformComponent>()) {
            if (entity.getComponent<Zero::NameComponent>().name.starts_with("money_")) {
                money = money + 1;
                LOG_INFO("You got $1! Now you have ${}", money);
                entity.getComponent<Zero::TransformComponent>().translation.y = -100.0f;
            }
        }
    }

    void onCollisionExit(Zero::Entity entity) override {
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