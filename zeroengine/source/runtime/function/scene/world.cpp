#include "runtime/function/scene/world.h"

namespace Zero {
    World::~World() {
        if (m_active_scene_indictor.has_value())
            m_scenes[m_active_scene_indictor.value()]->onRuntimeStop();
    }

    World& World::addScene(std::string_view scene_name) {
        if (m_scenes.contains(std::string(scene_name)))
            LOG_WARN("The scene with this name ({0}) has been exsited in the world!!!", scene_name);
        else
            m_scenes[std::string(scene_name)] = CreateRef<Scene>();
        return *this;
    }

    void World::removeScene(std::string_view scene_name) {
        if (m_scenes.contains(std::string(scene_name)))
            m_scenes.erase(std::string(scene_name));
        else
            LOG_WARN("The scene with this name ({0}) does not exsit in the world!!!", scene_name);
    }

    Ref<Scene> World::getScene(std::string_view scene_name) {
        ASSERT(m_scenes.contains(std::string(scene_name)),
               "The scene with this name does not exsit in the world!!!");
        return m_scenes[std::string(scene_name)];
    }

    World& World::setActiveScene(std::string_view scene_name) {
        if (m_scenes.contains(std::string(scene_name))) {
            if (m_active_scene_indictor.has_value())
                m_scenes[m_active_scene_indictor.value()]->onRuntimeStop();

            m_active_scene_indictor = scene_name;
            m_scenes[std::string(scene_name)]->onRuntimeStart();
        }
        else
            LOG_WARN("The scene with this name ({0}) does not exsit in the world!!!", scene_name);
        return *this;
    }

    Ref<Scene> World::getActiveScene() {
        ASSERT(m_active_scene_indictor.has_value(),
               "The world does not have a active scene!!!");
        return m_scenes[m_active_scene_indictor.value()];
    }

    void World::onResize(uint32_t width, uint32_t height) {
        if (m_active_scene_indictor.has_value())
            m_scenes[m_active_scene_indictor.value()]->onViewportResize(width, height);
    }

    void World::onUpdateEditor(TimeStep timestep, EditorCamera& editor_camera) {
        if (m_active_scene_indictor.has_value())
            m_scenes[m_active_scene_indictor.value()]->onUpdateEditor(timestep, editor_camera);
    }

    void World::onUpdateRuntime(TimeStep timestep) {
        if (m_active_scene_indictor.has_value())
            m_scenes[m_active_scene_indictor.value()]->onUpdateRuntime(timestep);
    }

    void World::onRuntimeStart() {
        if (m_active_scene_indictor.has_value())
            m_scenes[m_active_scene_indictor.value()]->onRuntimeStart();
    }
} // namespace Zero
