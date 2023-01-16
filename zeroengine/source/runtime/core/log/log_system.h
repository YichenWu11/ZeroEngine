#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>


namespace Zero {
    class LogSystem {
    public:
        static void init();

        static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };

} // namespace Zero

// Core log macros
#define LOG_TRACE(...) ::Zero::LogSystem::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Zero::LogSystem::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Zero::LogSystem::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Zero::LogSystem::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Zero::LogSystem::getLogger()->critical(__VA_ARGS__)
