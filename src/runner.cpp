#include "../include/runner.hpp"
#include "../include/logger.hpp"

#include <iostream>  

int Runner::run(const std::string &json_path, std::ostream &out, std::ostream &err) {
    Logger::instance().info("Starting Runner with file: {}", json_path);   
    try {
        Request request = parser.parse(json_path);
        checker.check(request);
        Response response = calculator.calculate(request);
        printer.print(request, response, out);

        Logger::instance().info("Calculation completed successfully");
        return 0;
    }
    catch (const std::exception &e) {
        Logger::instance().error("Error occurred: {}", e.what());
        err << "Error: " << e.what() << std::endl;
        return 1;
    }
}