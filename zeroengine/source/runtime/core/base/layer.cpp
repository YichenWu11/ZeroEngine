#include "runtime/core/base/layer.h"

namespace Zero {
    Layer::Layer(const std::string& debugName) :
        m_debug_name(debugName) {
    }

    Layer::~Layer() {
    }
} // namespace Zero