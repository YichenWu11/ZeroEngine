#pragma once

#include "runtime/core/interface/i_application.h"

namespace Zero {
    class BaseApplication : public IApplication {
    public:
        virtual int  initialize();
        virtual void finalize();

        // One cycle of the main loop
        virtual void tick();

        virtual bool isQuit();

    protected:
        // Flag if need quit the main loop of the application
        bool m_quit;
    };
} // namespace Zero