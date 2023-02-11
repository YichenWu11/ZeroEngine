#include "runtime/core/base/layer_stack.h"
#include "runtime/core/base/layer.h"

namespace Zero {
    LayerStack::LayerStack() {
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : m_layers) {
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
        m_layer_insert_index++;
        layer->onAttach();
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        m_layers.emplace_back(overlay);
        overlay->onAttach();
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert_index, layer);
        if (it != m_layers.begin() + m_layer_insert_index) {
            m_layers.erase(it);
            m_layer_insert_index--;
            layer->onDetach();
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(m_layers.begin() + m_layer_insert_index, m_layers.end(), overlay);
        if (it != m_layers.end()) {
            m_layers.erase(it);
            overlay->onDetach();
        }
    }

} // namespace Zero
