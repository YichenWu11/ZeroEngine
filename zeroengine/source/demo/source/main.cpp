#include "sandbox/sandbox.h"

int main(int /*argc*/, char** /*argv*/) {
    auto sandbox_app = createApp();

    sandbox_app->run();

    delete sandbox_app;
}
