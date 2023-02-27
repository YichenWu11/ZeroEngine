#pragma once

#include "runtime/function/render/camera_system/editor_camera.h"
#include "runtime/function/scene/scene.h"

namespace Zero {
    class World {
    public:
        World() = default;
        ~World();

        World(const World&)            = delete;
        World& operator=(const World&) = delete;

        World& addScene(std::string_view scene_name);
        void   removeScene(std::string_view scene_name);

        Ref<Scene> getScene(std::string_view scene_name);
        World&     setActiveScene(std::string_view scene_name);
        Ref<Scene> getActiveScene();

        void onResize(uint32_t width, uint32_t height);
        void onUpdateEditor(TimeStep timestep, EditorCamera& editor_camera);
        void onUpdateRuntime(TimeStep timestep);

    private:
        std::optional<std::string>        m_active_scene_indictor{std::nullopt};
        std::map<std::string, Ref<Scene>> m_scenes;
    };
} // namespace Zero