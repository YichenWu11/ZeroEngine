#include "sandbox/sandbox.h"

int main(int /*argc*/, char** /*argv*/) {
    Zero::LogSystem::init();

    LOG_INFO("zeroengine start");

    auto editor_app = createApplication();

    editor_app->run();

    delete editor_app;
}
