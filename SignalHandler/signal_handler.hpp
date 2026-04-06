#pragma once

#include <atomic>
#include <string>
#include <csignal>
#include <pthread.h>
#include "../include/logger.hpp"

class SignalHandler {
public:
    explicit SignalHandler(std::atomic<bool>& running, Logger& logger);

    void run();
    void stop();

private:
    std::atomic<bool>& running_;
    Logger& logger_;
    std::atomic<bool> thread_started_{false};
    pthread_t thread_id_{};
    sigset_t set_{};
};