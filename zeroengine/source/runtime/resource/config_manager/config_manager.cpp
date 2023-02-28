#include "runtime/resource/config_manager/config_manager.h"

namespace Zero {
    ConfigManager::ConfigManager() {
        m_root_folder   = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR));
        m_asset_folder  = m_root_folder / "asset";
        m_script_folder = m_root_folder / "script";
    }

    const std::filesystem::path& ConfigManager::getRootFolder() const { return m_root_folder; }
    const std::filesystem::path& ConfigManager::getAssetFolder() const { return m_asset_folder; }
    const std::filesystem::path& ConfigManager::getScriptFolder() const { return m_script_folder; }
} // namespace Zero