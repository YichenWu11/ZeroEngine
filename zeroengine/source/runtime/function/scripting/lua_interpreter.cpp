#include "runtime/function/scripting/lua_interpreter.h"
#include "runtime/function/scripting/lua_binder.h"

namespace Zero {
    LuaInterpreter::LuaInterpreter() {
        createLuaContextAndBindGlobals();
    }

    LuaInterpreter::~LuaInterpreter() {
        m_luaState.reset();
        m_isOk = false;
    }

    void LuaInterpreter::createLuaContextAndBindGlobals() {
        if (!m_luaState) {
            m_luaState = CreateScope<sol::state>();
            m_luaState->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

            LuaBinder::callBinders(*m_luaState);

            m_isOk = true;

            std::for_each(m_lua_scripts.begin(), m_lua_scripts.end(), [this](LuaScriptComponent* lsc) {
                if (!lsc->registerToLuaContext(*m_luaState))
                    m_isOk = false;
            });

            if (!m_isOk)
                LOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
        }
    }

    void LuaInterpreter::destroyLuaContext() {
        if (m_luaState) {
            std::for_each(m_lua_scripts.begin(), m_lua_scripts.end(), [this](LuaScriptComponent* lsc) {
                lsc->unregisterFromLuaContext();
            });

            m_luaState.reset();
            m_isOk = false;
        }
    }

    void LuaInterpreter::consider(LuaScriptComponent* p_toConsider) {
        if (m_luaState) {
            m_lua_scripts.push_back(p_toConsider);

            if (!p_toConsider->registerToLuaContext(*m_luaState))
                m_isOk = false;
        }
    }

    void LuaInterpreter::unconsider(LuaScriptComponent* p_toUnconsider) {
        if (m_luaState)
            p_toUnconsider->unregisterFromLuaContext();

        m_lua_scripts.erase(std::remove_if(m_lua_scripts.begin(), m_lua_scripts.end(), [p_toUnconsider](LuaScriptComponent* lsc) {
            return p_toUnconsider == lsc;
        }));

        refreshAll();
    }

    void LuaInterpreter::refreshAll() {
        destroyLuaContext();
        createLuaContextAndBindGlobals();
    }

    bool LuaInterpreter::isOk() const {
        return m_isOk;
    }

} // namespace Zero
