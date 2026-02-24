#include "../include/runner.hpp"
#include "../include/logger.hpp"

int Runner::run(int argc, const char* argv[], std::ostream &out) {
    checker.checkInputArgs(argc, argv);

    const std::string json_path = argv[1];
    Logger::instance().info("Starting Runner with file: %s", json_path.c_str());

    Request request = parser.parse(json_path);
    checker.checkRequest(request);

    Response response = calculator.calculate(request);
    printer.print(request, response, out);

    Logger::instance().info("Calculation completed successfully");
    return 0;
}