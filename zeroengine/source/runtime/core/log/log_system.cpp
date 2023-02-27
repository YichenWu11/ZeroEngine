#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "runtime/core/log/log_system.h"
#include "runtime/resource/config_manager/config_manager.h"

namespace Zero {
    std::shared_ptr<spdlog::logger> LogSystem::s_logger;

    void LogSystem::init() {
        auto log_path = ConfigManager::getInstance().getRootFolder() / "log/zeroengine.log";

        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path.string(), true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        s_logger = std::make_shared<spdlog::logger>("ZERO", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_logger);
        s_logger->set_level(spdlog::level::trace);
        s_logger->flush_on(spdlog::level::trace);
    }
} // namespace Zero
