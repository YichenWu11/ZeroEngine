#include "editor.h"

int main(int /*argc*/, char** /*argv*/) {
    Zero::LogSystem::init();

    LOG_INFO("zeroengine start");

    auto editor_app = Zero::createApplication();

    editor_app->run();

    delete editor_app;
}
