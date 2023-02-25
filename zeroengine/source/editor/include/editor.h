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

    Zero::Application* createApp() {
        Zero::RendererAPI::setEditorModeEnabled(true);
        return new Editor();
    }
} // namespace Zero