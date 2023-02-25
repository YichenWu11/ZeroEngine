#pragma once

#include <imgui.h>

#include "runtime/core/util/singleton.h"

namespace Zero {
    class Widgets : public Singleton<Widgets> {
    public:
        static void drawText(std::string_view text) {
            ImGui::Begin(std::string(text).c_str(), NULL, s_window_flag);
            ImGui::Text("%s", std::string(text).c_str());
            ImGui::End();
        }

        static void drawButton(
            const char*                  label,
            const Vector2&               size,
            const Color&                 normal_color,
            const Color&                 hover_color,
            const Color&                 active_color,
            const std::function<void()>& callback) {
            ImVec4 check_button_color = ImVec4{0.365f, 0.04f, 0.26f, 1.0f};

            ImGui::SetNextWindowSize(ImVec2(size.x + 18.0f, size.y + 18.0f));
            ImGui::Begin(label, NULL, s_window_flag);
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImVec4{normal_color.x, normal_color.y, normal_color.z, 0.40f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  ImVec4{hover_color.x, hover_color.y, hover_color.z, hover_color.w});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImVec4{active_color.x, active_color.y, active_color.z, active_color.w});

            if (ImGui::Button(label, ImVec2{size.x, size.y}))
                callback();

            ImGui::PopStyleColor(3);
            ImGui::End();
        }

        static void drawTextureButton(const char* label, ImTextureID user_texture_id, const Vector2& size, const std::function<void()>& callback) {
            ImGui::SetNextWindowSize(ImVec2(size.x + 18.0f, size.y + 18.0f));
            ImGui::Begin(label, NULL, s_window_flag);
            if (ImGui::ImageButton(user_texture_id, ImVec2{size.x, size.y}))
                callback();
            ImGui::End();
        }

    private:
        static const ImGuiWindowFlags s_window_flag{
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize};
    };
} // namespace Zero
