#include "../include/calculator.hpp"
#include "../include/errors.hpp"
#include "../include/logger.hpp"

Response Calculator::calculate(const Request &request) {
    Logger::instance().debug("Performing operation: {}", static_cast<int>(request.op));
    switch (request.op) {
        case OperationType::Add: return add(request.first, request.second); 
        case OperationType::Sub: return sub(request.first, request.second); 
        case OperationType::Mul: return mul(request.first, request.second); 
        case OperationType::Div: return div(request.first, request.second);
        case OperationType::Pow: return pow(request.first, request.second);
        case OperationType::Fact: return fact(static_cast<uint64_t>(request.first));
        default: throw Error("Incorect operation type");
    }
}

Response Calculator::add(int64_t a, int64_t b) {
    Response result{};
    result.result_is_u64 = false;
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) throw Error("Overflow");

    result.result_i64 = a + b;
    return result;
}

Response Calculator::sub(int64_t a, int64_t b) {
    Response result{};
    result.result_is_u64 = false;
    if ((b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b)) throw Error("Overflow");

    result.result_i64 = a - b;
    return result;
}

Response Calculator::mul(int64_t a, int64_t b) {
    Response result{};
    result.result_is_u64 = false;
    if (a == 0 || b == 0) {
        result.result_i64 = 0;
        return result;
    }

    if (a == -1 && b == INT64_MIN) throw Error("Overflow");
    if (b == -1 && a == INT64_MIN) throw Error("Overflow");
    
    int64_t tmp = a * b;
    if (tmp / b != a) throw Error("Overflow");
    
    result.result_i64 = tmp;
    return result;
}

Response Calculator::div(int64_t a, int64_t b) {
    Response result{};
    result.result_is_u64 = false;
    if (a == INT64_MIN && b == -1) throw Error("Overflow");

    result.result_i64 = a / b;
    return result;
}

Response Calculator::pow(int64_t a, int64_t b) {
    Response result{};
    result.result_is_u64 = false;

    result.result_i64 = 1;
    for (int64_t i = 0; i < b; ++i) {
        Response tmp = mul(result.result_i64, a);
        result.result_i64 = tmp.result_i64;
    }
    return result;
}

Response Calculator::fact(uint64_t a) {
    if (a <= 1) {
        return Response{.result_u64 = 1, .result_is_u64 = true};
    }

    Response result = fact(a - 1);

    if (result.result_u64 > UINT64_MAX / a) throw Error("Overflow");

    result.result_u64 *= a;
    return result;
}