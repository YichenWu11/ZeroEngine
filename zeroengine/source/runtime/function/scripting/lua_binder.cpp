#include "runtime/function/scripting/lua_binder.h"
#include "runtime/function/scripting/binder/lua_component_binder.h"
#include "runtime/function/scripting/binder/lua_entity_binder.h"
#include "runtime/function/scripting/binder/lua_globals_binder.h"
#include "runtime/function/scripting/binder/lua_math_binder.h"

namespace Zero {
    void LuaBinder::callBinders(sol::state& p_luaState) {
        auto& L = p_luaState;

        LuaMathBinder::bindMaths(L);
        LuaEntityBinder::bindEntity(L);
        LuaComponentBinder::bindComponent(L);
        LuaGlobalsBinder::bindGlobals(L);
    }
} // namespace Zero