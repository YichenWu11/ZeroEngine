#pragma once

#include "runtime/function/scene/scene.h"

namespace Zero {
    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void serialize(const std::filesystem::path& path);
        void serializerRuntime(const std::filesystem::path& path);

        bool deserialize(const std::filesystem::path& path);
        bool deserializerRuntime(const std::filesystem::path& path);

    private:
        Ref<Scene> m_scene;
    };
} // namespace Zero