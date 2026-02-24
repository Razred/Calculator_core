#include "../include/parser.hpp"
#include "../include/logger.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Request Parser::parse(const std::string &json_path) {
    Logger::instance().debug("Starting Parser with file: %s", json_path.c_str());

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

    try {
        request.op_str = data.at("op").get<std::string>();
    }
    catch (...) {
        throw Error("Not expected value type for key 'op'. Must be of type string");
    }

    if (request.op_str == "add" ) { request.op = OperationType::Add; request.op_str = "+"; }
    else if (request.op_str == "sub") { request.op = OperationType::Sub; request.op_str = "-"; }
    else if (request.op_str == "mul") { request.op = OperationType::Mul; request.op_str = "*"; }
    else if (request.op_str == "div") { request.op = OperationType::Div; request.op_str = "/"; }
    else if (request.op_str == "pow") { request.op = OperationType::Pow; request.op_str = "^"; }
    else if (request.op_str == "fact") { request.op = OperationType::Fact; request.op_str = "!"; }
    else throw Error("Not expected value " + request.op_str + " for key 'op'");

    if (!data.contains("second")) {
        Logger::instance().debug("Parsed request: first=%d, op=%s", request.first, request.op_str.c_str());
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

    Logger::instance().debug("Parser successfully completed: first=%d, second=%d, op=%s", request.first, request.second, request.op_str.c_str());
    return request;
}