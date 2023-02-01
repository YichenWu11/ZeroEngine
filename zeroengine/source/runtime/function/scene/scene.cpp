#include "runtime/function/scene/scene.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"

namespace Zero {
    Scene::Scene() {
        entt::entity entity = m_registry.create();
    }

    Scene::~Scene() {
    }

    Entity Scene::createEntity(const std::string& name) {
        Entity entity = {m_registry.create(), this};
        entity.addComponent<TransformComponent>();
        auto& name_component = entity.addComponent<NameComponent>();
        name_component.name  = (name.empty()) ? "Entity" : name;

        return entity;
    }

    void Scene::onUpdate(TimeStep timestep) {
        Camera* curr_camera = nullptr;
        Matrix  camera_transform{Matrix::Identity};
        {
            auto group = m_registry.view<TransformComponent, CameraComponent>();
            for (auto entity : group) {
                auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

                if (camera.is_current) {
                    curr_camera      = &camera.camera;
                    camera_transform = transform.getTransform();
                    break;
                }
            }
        }

        if (curr_camera) {
            Renderer2D::beginScene(*curr_camera, camera_transform);

            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawQuad(
                    transform.getTransform(),
                    sprite.color,
                    sprite.tex_index,
                    sprite.tiling_factor);
            }
        }

        Renderer2D::endScene();
    }

} // namespace Zero