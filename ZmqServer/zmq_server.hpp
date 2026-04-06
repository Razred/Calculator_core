#pragma once

#include <zmq.hpp>
#include <atomic>
#include <string>
#include "../include/runner.hpp"
#include "../include/logger.hpp"

class ZmqServer {
public:
    explicit ZmqServer(std::string client_addr, std::atomic<bool> &running, Logger &logger, DbConfig config);

    void run();

    void stop();

    bool isRunning() const { return running_.load(); }

    std::string getClientAddr() const { return client_addr_; }

private:
    zmq::context_t context_{1};
    zmq::socket_t  socket_{context_, zmq::socket_type::rep};

    std::atomic<bool> &running_;

    std::string client_addr_{};
    Runner      runner_;

    Logger &logger_;
};