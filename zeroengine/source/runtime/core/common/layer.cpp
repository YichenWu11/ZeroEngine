#include "runtime/core/common/layer.h"

namespace Zero {
    Layer::Layer(const std::string& debugName) :
        m_debug_name(debugName) {
    }

    Layer::~Layer() {
    }
} // namespace Zero