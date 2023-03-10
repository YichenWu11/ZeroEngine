#include "runtime/function/scene/scene.h"
#include "runtime/core/base/application.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"

namespace Zero {
    Entity Scene::createEntity(const std::string& name) {
        static uint32_t create_entity_count = 0;

        Entity entity = {m_registry.create(), this};
        entity.addComponent<TransformComponent>();
        auto& name_component = entity.addComponent<NameComponent>();
        name_component.name  = (name.empty()) ? ("Entity" + std::to_string(create_entity_count)) :
                                                name;

        create_entity_count++;

        return entity;
    }

    void Scene::destroyEntity(Entity entity) {
        m_registry.destroy(entity);
    }

    void Scene::onRuntimeStart() {
        // update lua scripts
        m_physics2d_manager = CreateScope<Physics2DManager>();

        {
            m_registry.view<LuaScriptComponent>().each([=](auto entity, LuaScriptComponent& lsc) {
                lsc.luaCall("onCreate");
            });
        }
    }

    void Scene::onRuntimeStop() {
        m_physics2d_manager.reset();
        // update lua scripts
        {
            m_registry.view<LuaScriptComponent>().each([=](auto entity, LuaScriptComponent& lsc) {
                lsc.luaCall("onDestroy");
            });
        }
    }

    void Scene::onUpdateRuntime(TimeStep timestep) {
        // update scripts
        {
            m_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
                if (!nsc.instance) {
                    nsc.instance           = nsc.instantiateScript();
                    nsc.instance->m_entity = Entity{entity, this};
                    nsc.instance->onCreate();
                }

                nsc.instance->onUpdate(timestep);
            });
        }

        // update lua scripts
        {
            m_registry.view<LuaScriptComponent>().each([=](auto entity, LuaScriptComponent& lsc) {
                lsc.luaCall("onUpdate", timestep.getSeconds());
            });
        }

        // render
        SceneCamera* curr_camera = nullptr;
        Matrix       camera_transform{Matrix::Identity};
        {
            auto view = m_registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view) {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                TransformComponent transform_cam;
                if (camera.is_fixed_rotation) {
                    transform_cam.translation = transform.translation;
                    transform_cam.scale       = transform.scale;
                }
                else {
                    transform_cam = transform;
                }

                if (camera.is_current) {
                    curr_camera      = &camera.camera;
                    camera_transform = transform_cam.getTransform();
                    break;
                }
            }
        }

        if (curr_camera) {
            Renderer2D::beginScene(*curr_camera, camera_transform);

            auto group = m_registry.group<>(entt::get<TransformComponent, SpriteComponent>, entt::exclude<TileComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

                Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
            }

            auto tile_group = m_registry.group<>(entt::get<TransformComponent, TileComponent>, entt::exclude<SpriteComponent>);
            for (auto entity : tile_group) {
                auto [transform, tile] = tile_group.get<TransformComponent, TileComponent>(entity);

                Renderer2D::drawTile(transform.getTransform(), tile, (int)entity);
            }
        }

        for (auto entity : m_submit_body_creation) {
            bool  is_dynamic = false, is_kinematic = false;
            auto& rigidbody2d = m_registry.get<Rigidbody2DComponent>(entity);
            auto& transform   = m_registry.get<TransformComponent>(entity);
            switch (rigidbody2d.type) {
                case Rigidbody2DComponent::BodyType::Dynamic:
                    is_dynamic = true;
                    break;
                case Rigidbody2DComponent::BodyType::Kinematic:
                    is_kinematic = true;
                    break;
                case Rigidbody2DComponent::BodyType::Static:
                    break;
            }
            rigidbody2d.runtime_body = reinterpret_cast<void*>(m_physics2d_manager->createBody((uint32_t)entity, transform.translation.x, transform.translation.y, transform.rotation.z, is_dynamic, is_kinematic, rigidbody2d.is_fixed_rotation, rigidbody2d.collider));
        }
        m_submit_body_creation.clear();

        auto rigidbody2d_view = m_registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto entity : rigidbody2d_view) {
            auto& transform = m_registry.get<TransformComponent>(entity);
            auto& rigidbody = m_registry.get<Rigidbody2DComponent>(entity);

            if (rigidbody.is_mutable) {
                if (rigidbody.linear_velocity.x != rigidbody.old_linear_velocity.x || rigidbody.linear_velocity.y != rigidbody.old_linear_velocity.y || rigidbody.angular_velocity != rigidbody.old_angular_velocity) {
                    m_physics2d_manager->setVelocity(rigidbody.runtime_body, rigidbody.linear_velocity.x, rigidbody.linear_velocity.y, rigidbody.angular_velocity);
                }
                if (transform.translation.x != rigidbody.old_translation.x || transform.translation.y != rigidbody.old_translation.y || transform.rotation.z != rigidbody.old_rotation) {
                    m_physics2d_manager->setTransform(rigidbody.runtime_body, transform.translation.x, transform.translation.y, transform.rotation.z);
                }
            }
        }
        m_physics2d_manager->update();
        for (auto entity : rigidbody2d_view) {
            auto& transform = m_registry.get<TransformComponent>(entity);
            auto& rigidbody = m_registry.get<Rigidbody2DComponent>(entity);
            if (rigidbody.show_box) {
                Color box_color;
                if (rigidbody.type == Rigidbody2DComponent::BodyType::Dynamic) {
                    box_color = Color(0.4, 0.1, 0.1, 0.4);
                }
                else {
                    box_color = Color(0.1, 0.1, 0.4, 0.4);
                }
                Renderer2D::drawQuad(transform.translation, Vector2(rigidbody.collider.getBoxWidth(), rigidbody.collider.getBoxHeight()), -180 * transform.rotation.z / 3.1415926535, box_color);
            }
            std::tie(rigidbody.old_linear_velocity.x, rigidbody.old_linear_velocity.y, rigidbody.old_angular_velocity) = m_physics2d_manager->getVelocity(rigidbody.runtime_body);
            std::tie(rigidbody.linear_velocity.x, rigidbody.linear_velocity.y, rigidbody.angular_velocity)             = std::make_tuple(rigidbody.old_linear_velocity.x, rigidbody.old_linear_velocity.y, rigidbody.old_angular_velocity);
            std::tie(rigidbody.old_translation.x, rigidbody.old_translation.y, rigidbody.old_rotation)                 = m_physics2d_manager->getTransform(rigidbody.runtime_body);
            std::tie(transform.translation.x, transform.translation.y, transform.rotation.z)                           = std::make_tuple(rigidbody.old_translation.x, rigidbody.old_translation.y, rigidbody.old_rotation);
            if (m_registry.all_of<NativeScriptComponent>(entity)) {
                auto& nas = m_registry.get<NativeScriptComponent>(entity);
                while (rigidbody.collider.hasCollisionEnterEvent()) {
                    nas.instance->onCollisionEnter(Entity{(entt::entity)rigidbody.collider.getCollisionEnterEvent(), this});
                }
                while (rigidbody.collider.hasCollisionExitEvent()) {
                    nas.instance->onCollisionExit(Entity{(entt::entity)rigidbody.collider.getCollisionExitEvent(), this});
                }
            }
            if (m_registry.all_of<LuaScriptComponent>(entity)) {
                auto& lsc = m_registry.get<LuaScriptComponent>(entity);
                while (rigidbody.collider.hasCollisionEnterEvent()) {
                    lsc.luaCall("onCollisionEnter", Entity{(entt::entity)rigidbody.collider.getCollisionEnterEvent(), this});
                }
                while (rigidbody.collider.hasCollisionExitEvent()) {
                    lsc.luaCall("onCollisionExit", Entity{(entt::entity)rigidbody.collider.getCollisionExitEvent(), this});
                }
            }
        }

        Renderer2D::endScene();
    }

    void Scene::onUpdateEditor(TimeStep timestep, EditorCamera& camera) {
        Renderer2D::beginScene(camera);

        auto group = m_registry.group<>(entt::get<TransformComponent, SpriteComponent>, entt::exclude<TileComponent>);
        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
        }

        auto tile_group = m_registry.group<>(entt::get<TransformComponent, TileComponent>, entt::exclude<SpriteComponent>);
        for (auto entity : tile_group) {
            auto [transform, tile] = tile_group.get<TransformComponent, TileComponent>(entity);

            Renderer2D::drawTile(transform.getTransform(), tile, (int)entity);
        }

        Renderer2D::endScene();
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height) {
        m_viewport_width  = width;
        m_viewport_height = height;

        // resize non-fixedAspectRatio cameras
        auto view = m_registry.view<CameraComponent>();
        for (auto& entity : view) {
            auto& camera_component = view.get<CameraComponent>(entity);
            if (!camera_component.is_fixed_aspectRatio) {
                camera_component.camera.setViewportSize(width, height);
            }
        }
    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.is_current)
                return Entity{entity, this};
        }
        return {};
    }

    Entity Scene::getEntityByName(std::string_view _name) {
        auto view = m_registry.view<NameComponent>();
        for (auto entity : view) {
            const auto& name = view.get<NameComponent>(entity);
            if (name.name == std::string(_name))
                return Entity{entity, this};
        }
        return {};
    }

    // ************************************************************************************************

    template <typename T>
    void Scene::onComponentAdded(Entity entity, T& component) {
    }

    template <>
    void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
    }

    template <>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
        component.camera.setViewportSize(m_viewport_width, m_viewport_height);
    }

    template <>
    void Scene::onComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component) {
    }

    template <>
    void Scene::onComponentAdded<TileComponent>(Entity entity, TileComponent& component) {
    }

    template <>
    void Scene::onComponentAdded<NameComponent>(Entity entity, NameComponent& component) {
    }

    template <>
    void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
    }

    template <>
    void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
        m_submit_body_creation.push_back(entity);
    }

    template <>
    void Scene::onComponentAdded<LuaScriptComponent>(Entity entity, LuaScriptComponent& component) {
        component.parent_entity = entity;
        Application::get().getLuaInterpreter()->consider(&component);
    }

    // ************************************************************************************************

    template <typename T>
    void Scene::onComponentErased(Entity entity, T& component) {
    }

    template <>
    void Scene::onComponentErased<TransformComponent>(Entity entity, TransformComponent& component) {
    }

    template <>
    void Scene::onComponentErased<CameraComponent>(Entity entity, CameraComponent& component) {
    }

    template <>
    void Scene::onComponentErased<SpriteComponent>(Entity entity, SpriteComponent& component) {
    }

    template <>
    void Scene::onComponentErased<TileComponent>(Entity entity, TileComponent& component) {
    }

    template <>
    void Scene::onComponentErased<NameComponent>(Entity entity, NameComponent& component) {
    }

    template <>
    void Scene::onComponentErased<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
    }

    template <>
    void Scene::onComponentErased<LuaScriptComponent>(Entity entity, LuaScriptComponent& component) {
        Application::get().getLuaInterpreter()->unconsider(&component);
    }

    template <>
    void Scene::onComponentErased<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
    }

} // namespace Zero