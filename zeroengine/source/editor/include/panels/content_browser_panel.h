#pragma once

namespace Zero {

    class ContentBrowserPanel {
    public:
        ContentBrowserPanel();

        void onImGuiRender();

    private:
        std::filesystem::path m_current_directory;
    };

} // namespace Zero