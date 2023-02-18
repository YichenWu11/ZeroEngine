#pragma once

#include "editor_layer.h"

namespace Zero {
    class Editor : public Zero::Application {
    public:
        Editor() {
            pushLayer(new EditorLayer());
        }

        ~Editor() {
        }
    };

    Zero::Application* createApplication() {
        Zero::RendererAPI::setEditorModeEnabled(true);
        return new Editor();
    }
} // namespace Zero