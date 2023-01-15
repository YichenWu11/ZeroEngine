#include "base_application.h"

namespace Zero {
    // Parse command line, read configuration, initialize all sub modules
    int BaseApplication::initialize() {
        m_quit = false;

        return 0;
    }

    // Finalize all sub modules and clean up all runtime temporary files.
    void BaseApplication::finalize() {
    }

    // One cycle of the main loop
    void BaseApplication::tick() {
    }

    bool BaseApplication::isQuit() {
        return m_quit;
    }
} // namespace Zero