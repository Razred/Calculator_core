#include "../include/parser.hpp"
#include "../include/logger.hpp"

Request Parser::parse(const nlohmann::json &json_data) {
    Logger::instance().debug("Starting Parser");

    if (!json_data.contains("first")) { throw Error("Key 'first' hasn't been found in json"); }

    if (!json_data.contains("op")) { throw Error("Key 'op' hasn't been found in json"); }

    Request request{};

    try {
        request.first = json_data.at("first").get<int64_t>();
    }
    catch (...) {
        throw Error("Not expected value type for key 'first'. Must be of type int");
    }

    try {
        request.op_str = json_data.at("op").get<std::string>();
    }
    catch (...) {
        throw Error("Not expected value type for key 'op'. Must be of type string");
    }

    if (request.op_str == "add" || request.op_str == "+") { request.op = OperationType::Add; request.op_str = "+"; }
    else if (request.op_str == "sub" || request.op_str == "-") { request.op = OperationType::Sub; request.op_str = "-"; }
    else if (request.op_str == "mul" || request.op_str == "*") { request.op = OperationType::Mul; request.op_str = "*"; }
    else if (request.op_str == "div" || request.op_str == "/") { request.op = OperationType::Div; request.op_str = "/"; }
    else if (request.op_str == "pow" || request.op_str == "^") { request.op = OperationType::Pow; request.op_str = "^"; }
    else if (request.op_str == "fact" || request.op_str == "!") { request.op = OperationType::Fact; request.op_str = "!"; }
    else throw Error("Not expected value " + request.op_str + " for key 'op'");

    if (!json_data.contains("second")) {
        Logger::instance().debug("Parsed request: first=%d, op=%s", request.first, request.op_str.c_str());
        request.has_second = false;
        return request;
    }

    try {
        request.second     = json_data.at("second").get<int64_t>();
        request.has_second = true;
    }
    catch (...) {
        throw Error("Not expected value type for key 'second'. Must be of type int");
    }

    Logger::instance().debug("Parser successfully completed: first=%d, second=%d, op=%s", request.first, request.second, request.op_str.c_str());
    return request;
}