#include "supervisor_runner.hpp"

SupervisorRunner::SupervisorRunner(AppConfig config) : config_(std::move(config)),
    logger_(Logger::instance()), zmq_server_(std::move(config_.server.client_addr), running_, logger_, config_.db),
    signal_handler_(running_, logger_) {}

SupervisorRunner::~SupervisorRunner() { shutdown(); }

void SupervisorRunner::run() {
    logger_.info("Starting Supervisor...");

    try {
        signal_thread_ = std::thread([this] { signal_handler_.run(); });

        server_thread_ = std::thread([this] { zmq_server_.run(); });

        if (signal_thread_.joinable()) { signal_thread_.join(); }

        logger_.info("Signal Handler finished, stopping...");
        zmq_server_.stop();

        if (server_thread_.joinable()) { server_thread_.join(); }

        logger_.info("Supervisor stop");
    }
    catch (const std::exception &e) {
        logger_.error("Supervisor error: {}", std::string(e.what()));
        shutdown();
        throw Error("");
    }
    catch (...) {
        logger_.error("Supervisor unknown error");
        shutdown();
        throw Error("");
    }
}

void SupervisorRunner::stop() {
    logger_.info("Supervisor stop requested");
    shutdown();
}

void SupervisorRunner::shutdown() {
    if (!running_.exchange(false)) { return; }

    signal_handler_.stop();
    zmq_server_.stop();

    if (signal_thread_.joinable()) { signal_thread_.join(); }

    if (server_thread_.joinable()) { server_thread_.join(); }
}
