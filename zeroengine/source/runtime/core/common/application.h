#pragma once

namespace Zero {
    class Application {
    public:
        Application();
        virtual ~Application();

        void run();
    };

    // TODO: to be defined
    Application* createApplication();
} // namespace Zero