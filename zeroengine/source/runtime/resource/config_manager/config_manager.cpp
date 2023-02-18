#include "runtime/resource/config_manager/config_manager.h"

namespace Zero {
    void ConfigManager::init() {
        m_root_folder  = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR));
        m_asset_folder = m_root_folder / "asset";
    }

    const std::filesystem::path& ConfigManager::getRootFolder() const { return m_root_folder; }
    const std::filesystem::path& ConfigManager::getAssetFolder() const { return m_asset_folder; }
} // namespace Zero