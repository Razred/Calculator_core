#include "../include/runner.hpp"
#include <iostream>  

int Runner::run(const std::string &json_path) {
    try {
        Request request = parser.parse(json_path);
        checker.check(request);
        Response response = calculator.calculate(request);
        printer.print(request, response);

        return 0;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}