#include "sandbox/sandbox.h"

int main(int /*argc*/, char** /*argv*/) {
    Zero::LogSystem::init();

    LOG_INFO("zeroengine start");

    auto sandbox_app = createApplication();

    sandbox_app->run();

    delete sandbox_app;
}
