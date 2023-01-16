#include "runtime/core/common/application.h"
#include "runtime/core/log/log_system.h"
#include "runtime/function/event/application_event.h"

namespace Zero {
    Application::Application() {
    }

    Application::~Application() {
    }

    void Application::run() {
        // WindowResizeEvent event(1080, 920);

        // if (event.isInCategory(EventCategoryApplication)) {
        //     LOG_TRACE(event.toString());
        // }

        // if (event.isInCategory(EventCategoryInput)) {
        //     LOG_TRACE(event.toString());
        // }

        while (true) {}
    }
} // namespace Zero
