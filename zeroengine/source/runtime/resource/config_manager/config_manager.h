#pragma once

namespace Zero {
    class ConfigManager {
    public:
        ConfigManager(const std::filesystem::path& game_root_path);

        const std::filesystem::path& getRootFolder() const;
        const std::filesystem::path& getAssetFolder() const;
        const std::filesystem::path& getScriptFolder() const;

        const std::filesystem::path& getGameRootFolder() const;
        const std::filesystem::path& getGameAssetFolder() const;
        const std::filesystem::path& getGameScriptFolder() const;

    private:
        std::filesystem::path m_root_folder;
        std::filesystem::path m_asset_folder;
        std::filesystem::path m_script_folder;

        std::filesystem::path m_game_root_folder;
        std::filesystem::path m_game_asset_folder;
        std::filesystem::path m_game_script_folder;
    };
} // namespace Zero