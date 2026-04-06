#include "signal_handler.hpp"

SignalHandler::SignalHandler(std::atomic<bool>& running, Logger& logger)
    : running_(running), logger_(logger) {}

void SignalHandler::run() {
    thread_id_ = pthread_self();

    sigemptyset(&set_);
    sigaddset(&set_, SIGINT);
    sigaddset(&set_, SIGTERM);
    sigaddset(&set_, SIGUSR1);

    if (pthread_sigmask(SIG_BLOCK, &set_, nullptr) != 0) {
        logger_.error("Failed to set signal mask");
        running_.store(false);
        return;
    }

    thread_started_.store(true);
    int sig = 0;
    int result = sigwait(&set_, &sig);

    if (result != 0) {
        logger_.error("sigwait failed with error: {}", std::to_string(result));
        running_.store(false);
        return;
    }

    logger_.info("Received signal {} stopping...", sig);
    running_.store(false);
}

void SignalHandler::stop() {
    if (!thread_started_.load()) {
        logger_.error("Signal handled thread was not started");
        return;
    }

    running_.store(false);
    logger_.info("Sending SIGUSR1 to signal handler");

    const int result = pthread_kill(thread_id_, SIGUSR1);
    if (result != 0) {
        logger_.error("Failed to stop Signal Handler thread");
        return;
    }
}
