#pragma once

#include "i_runtime_module.h"

namespace Zero {
    class IApplication : public IRuntimeModule {
    public:
        virtual int  initialize() = 0;
        virtual void finalize()   = 0;

        // One cycle of the main loop
        virtual void tick() = 0;

        virtual bool isQuit() = 0;
    };
} // namespace Zero