#include "../include/checker.hpp"
#include "../include/logger.hpp"
#include "../include/errors.hpp"

void Checker::checkInputArgs(int argc, const char* argv[]) const {
    if (argc != 2) {
        throw Error("Usage: ./calc input.json\n");
    }
    if (argv[1] == nullptr || argv[1][0] == '\0') {
        throw Error("Input file path is empty");
    }
}

void Checker::checkRequest(const Request &request) const {
    Logger::instance().debug("Starting Checker");
    if (request.op != OperationType::Fact && !request.has_second) throw Error("Cannot calculate because 'second' parameter is missed for binary operation");
        
    switch (request.op) {
        case OperationType::Div:
            if (request.second == 0) throw Error("Division by zero");
            break;
        case OperationType::Pow:
            if (request.second < 0) throw Error("Cannot pow by negative exponent");
            break;
        case OperationType::Fact:
            if (request.first < 0) throw Error("Value 'first' must be >= 0 for factorial");
            break;
        default: break;
    }
    Logger::instance().debug("Checker successfully completed");
}