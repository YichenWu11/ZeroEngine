#pragma once

#include <sol/sol.hpp>

namespace Zero {
    class LuaComponentBinder {
    public:
        static void bindComponent(sol::state& p_luaState);
    };
} // namespace Zero