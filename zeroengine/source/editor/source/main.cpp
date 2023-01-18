#include "sandbox/sandbox.h"

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ZERO_XSTR(s) ZERO_STR(s)
#define ZERO_STR(s) #s

int main(int /*argc*/, char** /*argv*/) {
    Zero::LogSystem::init();

    LOG_INFO("zeroengine start {}", 1);

    auto editor_app = Zero::createApplication();

    editor_app->run();

    delete editor_app;
}
