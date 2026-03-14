#include "../include/calculator.hpp"
#include "../include/errors.hpp"
#include "../include/logger.hpp"
#include "operations.hpp"

namespace {
void throwByStatus(operations::OperationalStatus status) {
    using operations::OperationalStatus;
    switch (status) {
        case OperationalStatus::AllRight:
            return;
        case OperationalStatus::DivisionByZero:
            throw Error("Division by zero");
        case OperationalStatus::Overflow:
            throw Error("Overflow");
        case OperationalStatus::ValidationError:
            throw Error("Validation error");
        case OperationalStatus::UnsupportedOperation:
            throw Error("Unsupported operation");
        case OperationalStatus::ParseError:
            throw Error("Parse error");
        default:
            throw Error("Unknown operation error");
    }
}

}

Response Calculator::calculate(const Request& request) {
    Logger::instance().debug("Performing operation: %d", static_cast<int>(request.op));

    operations::OperationalStatus status = operations::OperationalStatus::AllRight;

    Response response{};

    switch (request.op) {
        case OperationType::Add: {
            int64_t result = 0;
            if (!operations::add(request.first, request.second, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        case OperationType::Sub: {
            int64_t result = 0;
            if (!operations::sub(request.first, request.second, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        case OperationType::Mul: {
            int64_t result = 0;
            if (!operations::mul(request.first, request.second, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        case OperationType::Div: {
            int64_t result = 0;
            if (!operations::div(request.first, request.second, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        case OperationType::Pow: {
            int64_t result = 0;
            if (!operations::pow(request.first, request.second, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        case OperationType::Fact: {
            int64_t result = 0;
            if (request.first < 0) throw Error("Value 'first' must be >= 0 for factorial");
            if (!operations::fact(request.first, result, status)) throwByStatus(status);
            response.result_i64 = result;
            response.status = static_cast<int>(status);
            return response;
        }
        default:
            throw Error("Incorrect operation type");
    }
}