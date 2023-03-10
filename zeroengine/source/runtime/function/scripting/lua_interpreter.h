#pragma once

#include <sol/sol.hpp>

#include "runtime/function/scene/components.h"

namespace Zero {
    class LuaInterpreter {
    public:
        LuaInterpreter();
        ~LuaInterpreter();

        void createLuaContextAndBindGlobals();
        void destroyLuaContext();

        void consider(LuaScriptComponent* p_toConsider);
        void unconsider(LuaScriptComponent* p_toUnconsider);

        void refreshAll(); /* refresh all the scripts */

        bool isOk() const;

        sol::state& getLuaState() { return *m_luaState; }

    private:
        Scope<sol::state>                m_luaState;
        bool                             m_isOk;
        std::vector<LuaScriptComponent*> m_lua_scripts;
    };
} // namespace Zero