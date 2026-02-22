#include "../include/parser.hpp"
#include "../include/logger.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Request Parser::parse(const std::string &json_path) {
    Logger::instance().debug("Starting Parser with file: {}", json_path);

    if (json_path.empty()) throw Error("json path is empty");

    std::ifstream file(json_path);
    if (!file.is_open()) throw Error("Cannot opern json file: " + json_path);

    json data = json::parse(file);

    if (!data.contains("first")) { throw Error("Key 'first' hasn't been found in json"); }

    if (!data.contains("op")) { throw Error("Key 'op' hasn't been found in json"); }

    Request request{};

    try {
        request.first = data.at("first").get<int64_t>();
    }
    catch (...) {
        throw Error("Not expected value type for key 'first'. Must be of type int");
    }

    std::string op_str;
    try {
        op_str = data.at("op").get<std::string>();
    }
    catch (...) {
        throw Error("Not expected value type for key 'op'. Must be of type string");
    }

    if (op_str == "add") request.op = OperationType::Add;
    else if (op_str == "sub") request.op = OperationType::Sub;
    else if (op_str == "mul") request.op = OperationType::Mul;
    else if (op_str == "div") request.op = OperationType::Div;
    else if (op_str == "pow") request.op = OperationType::Pow;
    else if (op_str == "fact") request.op = OperationType::Fact;
    else throw Error("Not expected value " + op_str + " for key 'op'");

    if (!data.contains("second")) {
        Logger::instance().debug("Parsed request: first={}, op={}", request.first, op_str);
        request.has_second = false;
        return request;
    }

    try {
        request.second     = data.at("second").get<int64_t>();
        request.has_second = true;
    }
    catch (...) {
        throw Error("Not expected value type for key 'second'. Must be of type int");
    }

    Logger::instance().debug("Parser successfully completed: first={}, second={}, op={}", request.first, request.second, op_str);
    return request;
}