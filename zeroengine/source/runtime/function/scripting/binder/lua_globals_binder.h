#pragma once

#include <sol/sol.hpp>

namespace Zero {
    class LuaGlobalsBinder {
    public:
        static void bindGlobals(sol::state& p_luaState);
    };
} // namespace Zero