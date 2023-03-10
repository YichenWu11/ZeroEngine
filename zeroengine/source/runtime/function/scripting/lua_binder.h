#pragma once

#include <sol/sol.hpp>

namespace Zero {
    class LuaBinder {
    public:
        static void callBinders(sol::state& p_luaState);
    };
} // namespace Zero