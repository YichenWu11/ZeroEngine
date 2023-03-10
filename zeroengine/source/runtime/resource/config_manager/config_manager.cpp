#include "runtime/resource/config_manager/config_manager.h"

namespace Zero {
    ConfigManager::ConfigManager(const std::filesystem::path& game_root_path) {
        m_root_folder   = std::filesystem::path(ZE_XSTR(ROOT_DIR));
        m_asset_folder  = m_root_folder / "asset";
        m_script_folder = m_root_folder / "script";

        m_game_root_folder   = game_root_path;
        m_game_asset_folder  = game_root_path / "asset";
        m_game_script_folder = game_root_path / "script";
    }

    const std::filesystem::path& ConfigManager::getRootFolder() const { return m_root_folder; }
    const std::filesystem::path& ConfigManager::getAssetFolder() const { return m_asset_folder; }
    const std::filesystem::path& ConfigManager::getScriptFolder() const { return m_script_folder; }

    const std::filesystem::path& ConfigManager::getGameRootFolder() const { return m_game_root_folder; }
    const std::filesystem::path& ConfigManager::getGameAssetFolder() const { return m_game_asset_folder; }
    const std::filesystem::path& ConfigManager::getGameScriptFolder() const { return m_game_script_folder; }
} // namespace Zero