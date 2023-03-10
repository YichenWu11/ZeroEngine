#include "runtime/function/scripting/binder/lua_globals_binder.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/world.h"

namespace Zero {
    void LuaGlobalsBinder::bindGlobals(sol::state& p_luaState) {
        p_luaState.new_usertype<World>(
            "World",

            /* Method */
            "GetScene", &World::getScene,
            "GetActiveScene", &World::getActiveScene,
            "SetActiveScene", &World::setActiveScene,
            "AddScene", &World::addScene,
            "RemoveScene", &World::removeScene);

        p_luaState.new_usertype<Scene>(
            "Scene",

            /* Method */
            "GetPrimaryCameraEntity", [](Scene& scene) -> Entity { return scene.getPrimaryCameraEntity(); },
            "GetEntityByName", [](Scene& scene, std::string_view name) -> Entity { return scene.getEntityByName(name); });

        p_luaState.create_named_table("Debug");
        p_luaState.create_named_table("Input");

        p_luaState["Debug"]["Log"]        = [](const std::string& p_message) { LOG_TRACE(p_message); };
        p_luaState["Debug"]["LogInfo"]    = [](const std::string& p_message) { LOG_INFO(p_message); };
        p_luaState["Debug"]["LogWarning"] = [](const std::string& p_message) { LOG_WARN(p_message); };
        p_luaState["Debug"]["LogError"]   = [](const std::string& p_message) { LOG_ERROR(p_message); };

        p_luaState["Input"]["isKeyPressed"]         = [](int keycode) -> bool { return InputSystem::isKeyPressed(keycode); };
        p_luaState["Input"]["isMouseButtonPressed"] = [](int button) -> bool { return InputSystem::isMouseButtonPressed(button); };
        p_luaState["Input"]["getMousePosition"]     = []() -> Vector2 { return InputSystem::getMousePosition(); };
        p_luaState["Input"]["getMouseX"]            = []() -> float { return InputSystem::getMouseX(); };
        p_luaState["Input"]["getMouseY"]            = []() -> float { return InputSystem::getMouseY(); };
    }
} // namespace Zero