#pragma once

#include "runtime/core/util/singleton.h"

#define GET_CONFIG_MNGR() ::Zero::ConfigManager::getInstance()

namespace Zero {
    class ConfigManager : public Singleton<ConfigManager> {
    public:
        void init();

        const std::filesystem::path& getRootFolder() const;
        const std::filesystem::path& getAssetFolder() const;

    private:
        std::filesystem::path m_root_folder;
        std::filesystem::path m_asset_folder;
    };
} // namespace Zero