#pragma once
class Logger {
public:
    static Logger& instance();

    void setDebug();
    void setInfo();
    void setError();

    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void error(const char* fmt, ...);

private:
    Logger();
    Logger(const Logger &) = delete;
    Logger& operator=(const Logger &) = delete;

    ~Logger() = default;

    struct Impl;
    Impl *impl;
};