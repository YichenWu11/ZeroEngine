#include <iostream>

#include "runtime/core/interface/i_application.h"

using namespace Zero;

namespace Zero {
    extern IApplication* g_pApp;
}

int main(int /*argc*/, char** /*argv*/) {
    std::cout << "hello engine" << std::endl;

    int ret;

    if ((ret = g_pApp->initialize()) != 0) {
        printf("App Initialize failed, will exit now.");
        return ret;
    }

    while (!g_pApp->isQuit()) {
        g_pApp->tick();
    }

    g_pApp->finalize();

    return 0;
}