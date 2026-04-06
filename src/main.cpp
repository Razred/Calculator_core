#include "../SupervisorRunner/supervisor_runner.hpp"

int main() {
    AppConfig config;
    SupervisorRunner supervisor(config);

    try {
        supervisor.run();
    }
    catch (const std::exception &e) {
        Logger::instance().error("Fatal error: %s", e.what());
        return 1;
    }
    catch (...) {
        Logger::instance().error("Unknown fatal error");
        return 1;
    }

    return 0;
}