#include <imgui.h>
#include <imgui_internal.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/scene/components.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "panels/scene_hierarchy_panel.h"

#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Zero {
    static void drawVec3Control(const std::string& label, Vector3& values, float reset_value = 0.0f, float column_width = 100.0f) {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            values.x = reset_value;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            values.y = reset_value;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
            values.z = reset_value;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template <typename T, typename UIFunction>
    static void drawComponent(const std::string& name, Entity entity, UIFunction ui_function, bool remove_enable = true) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.hasComponent<T>()) {
            auto&  component               = entity.getComponent<T>();
            ImVec2 content_RegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4{0.2f, 0.024f, 0.08f, 1.0f});
            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::SameLine(content_RegionAvailable.x - line_height * 0.5f);

            if (ImGui::Button("+", ImVec2{line_height, line_height})) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool remove_component = false;

            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove Component"))
                    remove_component = true & remove_enable;
                ImGui::EndPopup();
            }

            if (open) {
                ui_function(component);
                ImGui::TreePop();
            }

            if (remove_component)
                entity.removeComponent<T>();
        }
    }

    // *********************************************************************************************************

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
        setContext(scene);
    }

    void SceneHierarchyPanel::setContext(const Ref<Scene>& context) {
        m_context = context;
    }

    void SceneHierarchyPanel::onImGuiRender() {
        {
            ImGui::Begin("Scene");

            m_context->m_registry.each([&](auto entityHandle) {
                Entity entity{entityHandle, m_context.get()};
                drawEntityNode(entity);
            });

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_selected_entity = {};

            // right-click on blank space
            if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
                if (ImGui::MenuItem("Create Empty Entity"))
                    m_context->createEntity("New Entity");

                ImGui::EndPopup();
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Inspector");

            if (m_selected_entity)
                drawComponents(m_selected_entity);

            ImGui::End();
        }
    }

    void SceneHierarchyPanel::drawEntityNode(Entity entity) {
        auto& name = entity.getComponent<NameComponent>().name;

        ImGuiTreeNodeFlags flags = ((m_selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::Selectable(name.c_str(), m_selected_entity == entity)) {
            m_selected_entity = entity;
        }

        bool entity_deleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity"))
                entity_deleted = true;

            ImGui::EndPopup();
        }

        if (entity_deleted) {
            m_context->destroyEntity(entity);
            if (m_selected_entity == entity)
                m_selected_entity = {};
        }
    }

    void SceneHierarchyPanel::drawComponents(Entity entity) {
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Transform")) {
                m_selected_entity.addComponent<TransformComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Camera")) {
                m_selected_entity.addComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite")) {
                m_selected_entity.addComponent<SpriteComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("NativeScript")) {
                m_selected_entity.addComponent<NativeScriptComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        // NameComponent
        drawComponent<NameComponent>(
            "Name", entity, [](auto& component) {
                auto& name = component.name;

                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strcpy_s(buffer, sizeof(buffer), name.c_str());

                if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                    if (!buffer[0]) buffer[0] = ' ';
                    name = std::string(buffer);
                }
            },
            false);

        // TransformComponent
        drawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            drawVec3Control("Position", component.translation);
            Vector3 rotation = {
                DirectX::XMConvertToDegrees(component.rotation.x),
                DirectX::XMConvertToDegrees(component.rotation.y),
                DirectX::XMConvertToDegrees(component.rotation.z)};
            drawVec3Control("Rotation", rotation);
            component.rotation = Vector3{
                DirectX::XMConvertToRadians(rotation.x),
                DirectX::XMConvertToRadians(rotation.y),
                DirectX::XMConvertToRadians(rotation.z)};
            drawVec3Control("Scale", component.scale, 1.0f);
        });

        // CameraComponent
        drawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            auto& camera = component.camera;

            ImGui::Checkbox("Current", &component.is_current);

            const char* projectionTypeStrings[]     = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (int i = 0; i < 2; i++) {
                    bool is_selected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], is_selected)) {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.setProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective) {
                float verticalFov = DirectX::XMConvertToDegrees(camera.getPerspectiveVerticalFOV());
                if (ImGui::DragFloat("Vertical FOV", &verticalFov))
                    camera.setPerspectiveVerticalFOV(DirectX::XMConvertToRadians(verticalFov));

                float perspectiveNear = camera.getPerspectiveNearClip();
                if (ImGui::DragFloat("Near", &perspectiveNear, 1.0f, 0.1f))
                    camera.setPerspectiveNearClip(perspectiveNear);

                float perspectiveFar = camera.getPerspectiveFarClip();
                if (ImGui::DragFloat("Far", &perspectiveFar, 1.0f, 0.1f))
                    camera.setPerspectiveFarClip(perspectiveFar);
            }

            if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                float orthoSize = camera.getOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize, 0.2f))
                    camera.setOrthographicSize(orthoSize);

                float orthoNear = camera.getOrthographicNearClip();
                if (ImGui::DragFloat("Near", &orthoNear))
                    camera.setOrthographicNearClip(orthoNear);

                float orthoFar = camera.getOrthographicFarClip();
                if (ImGui::DragFloat("Far", &orthoFar))
                    camera.setOrthographicFarClip(orthoFar);

                ImGui::Checkbox("Fixed Aspect Ratio", &component.is_fixed_aspectRatio);
            }
        });

        // SpriteComponent
        drawComponent<SpriteComponent>("Sprite", entity, [](auto& component) {
            ImGui::ColorEdit4("Modulate", reinterpret_cast<float*>(&component.color));

            ImGui::DragInt("TexID", reinterpret_cast<int*>(&component.tex_index), 1.0f, 0);
            component.tex_index = std::max(0, (int)component.tex_index);

            {
                ImGui::Image(
                    ImTextureID(GET_RENDER_CONTEXT().getTexAlloc().GetGpuHandle(component.tex_index).ptr),
                    ImVec2(50, 50));
            }

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    auto           tex_path =
                        GET_CONFIG_MNGR().getAssetFolder() / std::filesystem::path(path);
                    LOG_INFO("{0}", tex_path.string());
                    // TODO: Register Texture
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::DragFloat("Tiling", reinterpret_cast<float*>(&component.tiling_factor), 0.1f, 0.1f);
        });

        // NativeScriptComponent
        drawComponent<NativeScriptComponent>("NativeScript", entity, [](auto& component) {
        });
    }
} // namespace Zero