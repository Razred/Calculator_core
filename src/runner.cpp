#include "../include/runner.hpp"
#include "../include/logger.hpp"

namespace {
    nlohmann::json responseToJson(const Response& response) {
        nlohmann::json json;
        json["status"] = response.status;
        if (response.status == 0) {
            json["result"] = response.result_i64;
        }

        return json;
    }
}

int Runner::run(const nlohmann::json &req, nlohmann::json &recv, std::ostream &out) {
    Logger::instance().info("Starting Runner with json file");

    Request request = parser.parse(req);
    checker.checkRequest(request);

    std::optional<int64_t> second =
    request.has_second ? std::optional<int64_t>(request.second) : std::nullopt;

    auto cache = database.findInCache(request.first, second, request.op_str[0]);

    Response response{};
    if (cache.has_value()) {
        response.result_i64 = cache->result.value_or(0);
        response.status = cache->status;
    }
    else {
        response = calculator.calculate(request);
        std::optional<int64_t> res = (response.status == 0) ? std::optional<int64_t>(response.result_i64) : std::nullopt;
        database.saveOperation(request.first, second, request.op_str[0], res, response.status);
    }

    recv = responseToJson(response);

    printer.print(request, response, out);
    Logger::instance().info("Calculation completed successfully");
    return 0;
}