#include "zmq_server.hpp"

#include <nlohmann/json.hpp>

using nlohmann::json;

ZmqServer::ZmqServer(std::string client_addr, std::atomic<bool> &running, Logger &logger, DbConfig config) :
    running_(running), client_addr_(std::move(client_addr)), runner_(config), logger_(logger) {}

void ZmqServer::run() {
    bool expected = false;
    if (!running_.compare_exchange_weak(expected, true)) {
        logger_.info("Server is already running");
        return;
    }

    logger_.info("Starting ZMQ server on address: %s", getClientAddr());
    try {
        socket_.bind(getClientAddr());

        while (running_.load()) {
            try {
                zmq::message_t     request;
                zmq::recv_result_t recv_result = socket_.recv(request, zmq::recv_flags::none);

                if (!recv_result) {
                    if (!running_.load()) {
                        logger_.info("Server is stopping, exiting receive loop");
                        break;
                    }
                    continue;
                }

                json req = json::parse(request.to_string());
                json resp;
                try {
                    runner_.run(req, resp);
                }
                catch (const std::exception &e) {
                    logger_.error("Error processing request: {}", e.what());
                    resp = json({"error", e.what()});
                }

                zmq::message_t response(resp.dump());
                socket_.send(response, zmq::send_flags::none);
            }
            catch (const zmq::error_t &e) {
                if (e.num() == ETERM || e.num() == ENOTSOCK) {
                    logger_.info("Socket closed, exiting loop");
                    break;
                }
                logger_.error("ZMQ error: {}", e.what());
            }
        }
        socket_.unbind(getClientAddr());
    }
    catch (const zmq::error_t &e) {
        logger_.error("Failed to bind socket: %s", e.what());
    }
}

void ZmqServer::stop() {
    bool expected = true;
    if (!running_.compare_exchange_weak(expected, false)) {
        logger_.info("Server is already stopped");
        return;
    }

    logger_.info("Stopping ZMQ server");

    try {
        socket_.close();
    }
    catch (const zmq::error_t &e) {
        logger_.error("Failed to close socket: %s", e.what());
    }

    try {
        context_.shutdown();
    }
    catch (...) {
        logger_.error("Failed to shutdown context");
    }
}