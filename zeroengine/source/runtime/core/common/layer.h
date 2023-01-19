#pragma once

#include "runtime/function/event/event.h"

namespace Zero {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}

        const std::string& getName() const { return m_debug_name; }

    protected:
        std::string m_debug_name;
    };
} // namespace Zero