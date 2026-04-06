#pragma once

#include "../SignalHandler/signal_handler.hpp"
#include "../ZmqServer/zmq_server.hpp"
#include "../include/config.hpp"

#include <thread>

class SupervisorRunner {
public:
    explicit SupervisorRunner(AppConfig config);

    ~SupervisorRunner();

    void run();
    void stop();

private:
    void shutdown();

private:
    AppConfig config_;

    Logger &logger_;
    std::atomic<bool> running_{false};
    ZmqServer zmq_server_;
    SignalHandler signal_handler_;

    std::thread signal_thread_;
    std::thread server_thread_;
};