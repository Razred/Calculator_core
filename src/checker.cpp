#include "../include/checker.hpp"
#include "../include/logger.hpp"
#include "../include/errors.hpp"

void Checker::check(const Request &request) {
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