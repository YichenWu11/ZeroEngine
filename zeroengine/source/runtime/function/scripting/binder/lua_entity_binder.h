#pragma once

#include <sol/sol.hpp>

namespace Zero {
    class LuaEntityBinder {
    public:
        static void bindEntity(sol::state& p_luaState);
    };
} // namespace Zero