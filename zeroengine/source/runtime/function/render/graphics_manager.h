#pragma once

#include "runtime/core/interface/i_runtime_module.h"

namespace Zero {
    class GraphicsManager : public IRuntimeModule {
    public:
        virtual ~GraphicsManager() = default;
    };
} // namespace Zero