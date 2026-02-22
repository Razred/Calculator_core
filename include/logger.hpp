#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>
#include <vector>

class Logger {
public:
    static spdlog::logger & instance() {
        static std::shared_ptr<spdlog::logger> logger = create();
        return *logger;
    }

private:
    static std::shared_ptr<spdlog::logger> create() {

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("calc.log", true);
        file_sink->set_level(spdlog::level::trace);

        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

        auto logger = std::make_shared<spdlog::logger>("calc", sinks.begin(), sinks.end());
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::err);
        spdlog::register_logger(logger);

        return logger;
    }

    Logger() = default;
};