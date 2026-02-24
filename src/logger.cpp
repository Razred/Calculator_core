#include "../include/logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>
#include <vector>
#include <cstdarg>
#include <sstream>

namespace {

std::string vFormat(const char* fmt, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);

    const int n = std::vsnprintf(nullptr, 0, fmt, ap_copy);
    va_end(ap_copy);

    if (n <= 0) return {};

    std::string s;
    s.resize(static_cast<size_t>(n) + 1);
    std::vsnprintf(s.data(), s.size(), fmt, ap);
    s.pop_back();
    return s;
}

}

struct Logger::Impl {
    std::shared_ptr<spdlog::logger> logger;
};

Logger & Logger::instance() {
    static Logger lg;
    return lg;
}

Logger::Logger() : impl(new Impl()) {
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

    impl->logger = logger;
}

void Logger::setDebug() { impl->logger->set_level(spdlog::level::debug); }
void Logger::setInfo() { impl->logger->set_level(spdlog::level::info); }
void Logger::setError() { impl->logger->set_level(spdlog::level::err); }

void Logger::debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::string msg = vFormat(fmt, ap);
    impl->logger->debug(msg);
}

void Logger::info(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::string msg = vFormat(fmt, ap);
    impl->logger->info(msg);
}

void Logger::error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::string msg = vFormat(fmt, ap);
    impl->logger->error(msg);
}