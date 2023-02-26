#include <imgui.h>

#include "runtime/core/base/application.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "panels/content_browser_panel.h"

namespace Zero {
    static const int s_dir_icon  = 1;
    static const int s_file_icon = 2;

    ContentBrowserPanel::ContentBrowserPanel() :
        m_current_directory(GET_CONFIG_MNGR().getAssetFolder()) {
    }

    void ContentBrowserPanel::onImGuiRender() {
        ImGui::Begin("Browser");

        if (m_current_directory != std::filesystem::path(GET_CONFIG_MNGR().getAssetFolder())) {
            if (ImGui::Button("<-")) {
                m_current_directory = m_current_directory.parent_path();
            }
        }

        static float padding       = 16.0f;
        static float thumbnailSize = 64.0f;
        float        cellSize      = thumbnailSize + padding;

        float panelWidth  = ImGui::GetContentRegionAvail().x;
        int   columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (auto& directoryEntry : std::filesystem::directory_iterator(m_current_directory)) {
            const auto& path           = directoryEntry.path();
            auto        relativePath   = std::filesystem::relative(path, GET_CONFIG_MNGR().getAssetFolder());
            std::string filenameString = relativePath.filename().string();

            ImGui::PushID(filenameString.c_str());
            int icon = directoryEntry.is_directory() ? s_dir_icon : s_file_icon;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton(
                ImTextureID(GET_RENDER_CONTEXT().getTexAlloc().GetGpuHandle(icon).ptr),
                {thumbnailSize, thumbnailSize});

            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (directoryEntry.is_directory())
                    m_current_directory /= path.filename();
            }
            ImGui::TextWrapped("%s", filenameString.c_str());

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);

        // TODO: status bar
        ImGui::End();
    }
} // namespace Zero