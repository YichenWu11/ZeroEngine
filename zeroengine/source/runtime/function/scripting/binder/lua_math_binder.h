#pragma once

#include <sol/sol.hpp>

namespace Zero {
    class LuaMathBinder {
    public:
        static void bindMaths(sol::state& p_luaState);
    };
} // namespace Zero