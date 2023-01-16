#include <iostream>

#include "runtime/core/common/application.h"
#include "runtime/core/log/log_system.h"

#include "runtime/function/event/key_event.h"

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ZERO_XSTR(s) ZERO_STR(s)
#define ZERO_STR(s) #s

class SandBox : public Zero::Application {
public:
    SandBox() {
    }

    ~SandBox() {
    }
};

namespace Zero {
    Application* createApplication() {
        return new SandBox();
    }
} // namespace Zero

int main(int /*argc*/, char** /*argv*/) {
    Zero::LogSystem::init();

    LOG_INFO("zeroengine start {}", 1);

    auto editor_app = Zero::createApplication();

    editor_app->run();

    delete editor_app;

    return 0;
}